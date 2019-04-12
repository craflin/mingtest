
#include <gtest/gtest.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#endif

#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

namespace {

struct Test
{
    const char* name;
    void (*func)();
    int duration;
    std::list<std::string> failures;
};

struct Suite
{
    const char* suite;
    std::list<Test> tests;
    int duration;
    size_t failures;
};

std::list<Suite>* _suites = 0;
Test* _currentTest = 0;

bool match(const char* pat, const char* str)
{
    const char* s, * p;
    bool star = false;
loopStart:
    for (s = str, p = pat; *s; ++s, ++p)
    {
        switch (*p)
        {
        case '?':
            break;
        case '*':
            star = true;
            str = s, pat = p;
            do { ++pat; } while (*pat == '*');
            if (!*pat) return true;
            goto loopStart;
        default:
            if (*s != *p) goto starCheck;
            break;
        }
    }
    while (*p == '*') ++p;
    return !*p;
starCheck:
    if (!star) return false;
    str++;
    goto loopStart;
}

int time()
{
#ifdef _WIN32
  return (int)GetTickCount();
#else
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (int)(ts.tv_sec * 1000) + (int)(ts.tv_nsec / 1000000);
#endif
}

std::string executableName()
{
    // todo: cache result somewhere (thread-safe)
#ifdef _WIN32
    char path[MAX_PATH + 1];
    DWORD len = GetModuleFileNameA(NULL, path, sizeof(path));
    if (len == sizeof(path))
        return std::string();
#else
    char procfile[22];
    char path[PATH_MAX + 1] = {0};
#ifdef __linux__
    sprintf(procfile, "/proc/%d/exe", getpid());
#else
#error not implemented
#endif
    if (readlink(procfile, path, sizeof(path)) == -1)
        return std::string();
#endif
    std::string pathStr(path);
    size_t n = pathStr.find_last_of("\\/");
    if (n != std::string::npos)
        return pathStr.substr(n + 1);
    return pathStr;
}

}

namespace mingtest {

int listTests()
{
    if (_suites)
        for (std::list<Suite>::iterator i = _suites->begin(), end = _suites->end(); i != end; ++i)
        {
            Suite& suite = *i;
            for (std::list<Test>::iterator i = suite.tests.begin(), end = suite.tests.end(); i != end; ++i)
            {
                Test& test = *i;
                printf("%s.%s\n", suite.suite, test.name);
            }
        }
    return EXIT_SUCCESS;
}

int run(const char* filter, const char* outputFile_)
{
    struct Print
    {
        static std::string testUnit(size_t n)
        {
            return n == 1 ? "test" : "tests";
        }
        static std::string testCaseUnit(size_t n)
        {
            return n == 1 ? "test case" : "test cases";
        }
    };

    std::string outputFile;
    if (outputFile_)
        outputFile = outputFile_;
    else
    {
        char* x = getenv("GTEST_OUTPUT");
        if(x && strncmp(x, "xml:", 4) == 0 && x[4])
        {
            outputFile = x + 4;
            if(strchr("/\\", outputFile[outputFile.length() - 1]))
            {
                std::string fileName = executableName();;
                size_t n = fileName.rfind('.');
                if (n != std::string::npos)
                    fileName = fileName.substr(0, n);
                fileName += ".xml";
                outputFile += fileName;
            }
        }
    }

    std::list<Suite> activeSuites;
    size_t activeTests = 0;
    if (_suites)
        for (std::list<Suite>::iterator i = _suites->begin(), end = _suites->end(); i != end; ++i)
        {
            Suite& suite = *i;
            Suite* activeSuite = 0;
            for (std::list<Test>::iterator i = suite.tests.begin(), end = suite.tests.end(); i != end; ++i)
            {
                Test& test = *i;
                if (!filter || match(filter, (std::string(suite.suite) + "." + test.name).c_str()))
                {
                    if (!activeSuite)
                    {
                        activeSuites.push_back(Suite());
                        activeSuite = &activeSuites.back();
                        activeSuite->suite = suite.suite;
                    }
                    activeSuite->tests.push_back(test);
                    ++activeTests;
                }
            }
        }

    std::cout << "[==========] Running " << activeTests << " " << Print::testUnit(activeTests) << " from " << activeSuites.size() << " " << Print::testCaseUnit(activeSuites.size()) << "." << std::endl;
    std::cout << "[----------] Global test environment set-up." << std::endl;

    std::list<std::string> failedTests;
    int start = time();
    for (std::list<Suite>::iterator i = activeSuites.begin(), end = activeSuites.end(); i != end; ++i)
    {
        Suite& suite = *i;
        suite.failures = 0;
        std::cout << "[----------] " << suite.tests.size() << " " << Print::testUnit(suite.tests.size()) << " from " << suite.suite << std::endl;
        int start = time();
        for (std::list<Test>::iterator i = suite.tests.begin(), end = suite.tests.end(); i != end; ++i)
        {
            Test& test = *i;
            std::cout << "[ RUN      ] " << suite.suite << "." << test.name << std::endl;
            _currentTest = &test;
            int start = time();
            if (debugger())
            {
                try
                {
                    test.func();
                }
                catch (...)
                {
                    test.failures.push_back("uncaught exception");
                }
            }
            else
                test.func();
            _currentTest = 0;
            test.duration = time() - start;
            if (!test.failures.empty())
            {
                std::cout << "[  FAILED  ] " << suite.suite << "." << test.name << " (" << test.duration << " ms)" << std::endl;
                failedTests.push_back(std::string(suite.suite) + "." + test.name);
                ++suite.failures;
            }
            else
                std::cout << "[       OK ] " << suite.suite << "." << test.name << " (" << test.duration << " ms)" << std::endl;
        }
        suite.duration = time() - start;
        std::cout << "[----------] " << suite.tests.size() << " " << Print::testUnit(suite.tests.size()) << " from " << suite.suite << " (" << suite.duration << " ms total)" << std::endl << std::endl;
    }
    int duration = time() - start;

    std::cout << "[----------] Global test environment tear-down" << std::endl;
    std::cout << "[==========] " << activeTests << " " << Print::testUnit(activeTests) << " from " << activeSuites.size() << " " << Print::testCaseUnit(activeSuites.size()) << " ran. (" << duration << " ms total)" << std::endl;

    std::cout << "[  PASSED  ] " << (activeTests - failedTests.size()) << " " << Print::testUnit(activeTests - failedTests.size()) << "." << std::endl;

    if (!failedTests.empty())
    {
        std::cout << "[  FAILED  ] " << failedTests.size() << " " << Print::testUnit(failedTests.size()) << ", listed below:" << std::endl;
        for (std::list<std::string>::iterator i = failedTests.begin(), end = failedTests.end(); i != end; ++i)
            std::cout << "[  FAILED  ] " << *i << std::endl;

        if (failedTests.size() == 1)
            std::cout << std::endl << failedTests.size() << " FAILED TEST" << std::endl;
        else
            std::cout << std::endl << failedTests.size() << " FAILED TESTS" << std::endl;
    }

    std::cout << std::endl;

    if(!outputFile.empty())
    {
        std::ofstream file;
        file.open(outputFile.c_str());
        file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        char date[64];
        time_t now = time(0);
        struct tm* time = localtime(&now);
        strftime(date, sizeof(date), "%Y-%m-%dT%H:%M:%S", time);
        file << "<testsuites tests=\"" << activeTests << "\" failures=\"" << failedTests.size() << "\" disabled=\"0\" errors=\"0\" timestamp=\"" << date << "\" time=\"" << (duration / 1000.0) << "\" name=\"AllTests\">" << std::endl;
        for (std::list<Suite>::iterator i = activeSuites.begin(), end = activeSuites.end(); i != end; ++i)
        {
            Suite& suite = *i;
            file << "<testsuite name=\"" << suite.suite << "\" tests=\"" << suite.tests.size() << "\" failures=\"" << suite.failures << "\" disabled=\"0\" errors=\"0\" time=\"" << (suite.duration / 1000.0) << "\">" << std::endl;
            for (std::list<Test>::iterator i = suite.tests.begin(), end = suite.tests.end(); i != end; ++i)
            {
                Test& test = *i;
                file << "<testcase name=\"" << test.name << "\" status=\"run\" time=\"" << (test.duration / 1000.0) << "\" classname=\"" << suite.suite << "\">" << std::endl;
                for (std::list<std::string>::iterator i = test.failures.begin(), end = test.failures.end(); i != end; ++i)
                {
                    const std::string& failure = *i;
                    file << "<failure message=\"" << failure << "\" type=\"\"><![CDATA[" << failure << "]]></failure>" << std::endl;
                }
                file << "</testcase>" << std::endl;
            }
            file << "</testsuites>" << std::endl;
        }
        file.close();
    }

    return failedTests.empty() ? 0 : 1;
}

void add(const char* suite_, const char* name, void (*func)())
{
    static std::list<Suite> suites;
    Suite* suite = 0;
    for (std::list<Suite>::iterator i = suites.begin(), end = suites.end(); i != end; ++i)
        if (strcmp(i->suite, suite_) == 0)
        {
            suite = &*i;
            break;
        }
    if (!suite)
    {
        suites.push_back(Suite());
        suite = &suites.back();
        suite->suite = suite_;
    }
    Test test = {name, func};
    suite->tests.push_back(test);
    _suites = &suites;
}

void fail(const char* file, int line, const char* expression)
{
    std::stringstream error;
#ifdef _MSC_VER
    error << file << "(" << line << "): error: " << expression << " failed";
#else
    error << file << ":" << line << " error: " << expression << " failed";
#endif
    std::cerr << error.str() << std::endl;
    if (_currentTest)
        _currentTest->failures.push_back(error.str());
}

bool debugger()
{
    return false;
}

}
