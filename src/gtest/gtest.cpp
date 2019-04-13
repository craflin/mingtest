
#include <gtest/gtest.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#include <linux/limits.h>
#include <mcheck.h>
#endif

#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstring>

namespace {

struct TestData
{
    mingtest::Test* test;
    int duration;
    std::list<std::string> failures;
};

struct Suite
{
    std::list<TestData> tests;
    int duration;
    size_t failures;
};

mingtest::Test* _tests = 0;
TestData* _currentTestData = 0;

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
    for (Test* test = _tests; test; test = test->next)
        printf("%s.%s\n", test->suite, test->name);
    return EXIT_SUCCESS;
}

int run(const char* filter, const char* outputFile_)
{
#ifdef _WIN32
    #ifdef _DEBUG
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif
#else
    mcheck(NULL);
#endif

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
        char* gunit_output = getenv("GTEST_OUTPUT");
        if(gunit_output && strncmp(gunit_output, "xml:", 4) == 0 && gunit_output[4])
        {
            outputFile = gunit_output + 4;
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

    std::map<std::string, Suite> activeSuites;
    size_t activeTests = 0;
    for (Test* test = _tests; test; test = test->next)
    {
        if (!filter || match(filter, (std::string(test->suite) + "." + test->name).c_str()))
        {
            TestData testData = {test};
            activeSuites[test->suite].tests.push_back(testData);
            ++activeTests;
        }
    }

    std::cout << "[==========] Running " << activeTests << " " << Print::testUnit(activeTests) << " from " << activeSuites.size() << " " << Print::testCaseUnit(activeSuites.size()) << "." << std::endl;
    std::cout << "[----------] Global test environment set-up." << std::endl;

    std::list<std::string> failedTests;
    int start = time();
    for (std::map<std::string, Suite>::iterator i = activeSuites.begin(), end = activeSuites.end(); i != end; ++i)
    {
        const std::string& suiteName = i->first;
        Suite& suite = i->second;
        suite.failures = 0;
        std::cout << "[----------] " << suite.tests.size() << " " << Print::testUnit(suite.tests.size()) << " from " << suiteName << std::endl;
        int start = time();
        for (std::list<TestData>::iterator i = suite.tests.begin(), end = suite.tests.end(); i != end; ++i)
        {
            TestData& testData = *i;
            std::cout << "[ RUN      ] " << suiteName << "." << testData.test->name << std::endl;
            _currentTestData = &testData;
            int start = time();
            if (debugger())
            {
                try
                {
                    testData.test->func();
                }
                catch (...)
                {
                    fail(testData.test->file, testData.test->line, "uncaught exception");
                }
            }
            else
                testData.test->func();
#if defined(_WIN32) && defined(_DEBUG)
            if (!_CrtCheckMemory())
                fail(testData.test->file, testData.test->line, "detected memory corruption");
#else
            mcheck_check_all();
#endif
            _currentTestData = 0;
            testData.duration = time() - start;
            if (!testData.failures.empty())
            {
                std::cout << "[  FAILED  ] " << suiteName << "." << testData.test->name << " (" << testData.duration << " ms)" << std::endl;
                failedTests.push_back(suiteName + "." + testData.test->name);
                ++suite.failures;
            }
            else
                std::cout << "[       OK ] " << suiteName << "." << testData.test->name << " (" << testData.duration << " ms)" << std::endl;
        }
        suite.duration = time() - start;
        std::cout << "[----------] " << suite.tests.size() << " " << Print::testUnit(suite.tests.size()) << " from " << suiteName << " (" << suite.duration << " ms total)" << std::endl << std::endl;
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
        for (std::map<std::string, Suite>::iterator i = activeSuites.begin(), end = activeSuites.end(); i != end; ++i)
        {
            const std::string& suiteName = i->first;
            Suite& suite = i->second;
            file << "<testsuite name=\"" << suiteName << "\" tests=\"" << suite.tests.size() << "\" failures=\"" << suite.failures << "\" disabled=\"0\" errors=\"0\" time=\"" << (suite.duration / 1000.0) << "\">" << std::endl;
            for (std::list<TestData>::iterator i = suite.tests.begin(), end = suite.tests.end(); i != end; ++i)
            {
                TestData& testData = *i;
                file << "<testcase name=\"" << testData.test->name << "\" status=\"run\" time=\"" << (testData.duration / 1000.0) << "\" classname=\"" << suiteName << "\">" << std::endl;
                for (std::list<std::string>::iterator i = testData.failures.begin(), end = testData.failures.end(); i != end; ++i)
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

void add(Test& test)
{
    test.next = _tests;
    _tests = &test;
}

void fail(const char* file, int line, const char* expression)
{
    std::stringstream error;
#ifdef _MSC_VER
    error << file << "(" << line << "): error: " << expression ;
#else
    error << file << ":" << line << " error: " << expression;
#endif
    std::cerr << error.str() << std::endl;
    if (_currentTestData)
        _currentTestData->failures.push_back(error.str());
}

bool debugger()
{
    return false;
}

}
