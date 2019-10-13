
#include <gtest/gtest.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#include <linux/limits.h>
#include <mcheck.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include <list>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstring>
#include <cstdlib>

namespace {

struct TestData
{
    mingtest::Test* test;
    int duration;
    std::list<std::string> failures;
    bool skipped;

    TestData(mingtest::Test* test) : test(test), duration(-1), skipped(false) {}
};

struct Suite
{
    std::list<TestData> tests;
    int duration;
    size_t failures;
    size_t skipped;

    Suite() : duration(-1), failures(0), skipped(0) {}
};

mingtest::Test* _tests = 0;
TestData* _currentTestData = 0;
bool _debugger = false;

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

std::string basename(const std::string& path)
{
    size_t n = path.find_last_of("\\/");
    if (n != std::string::npos)
        return path.substr(n + 1);
    return path;
}

std::string dirname(const std::string& path)
{
    size_t n = path.find_last_of("\\/");
    if (n != std::string::npos)
        return path.substr(0, n);
    return ".";
}

bool exists(const std::string& path)
{
#ifdef _WIN32
    return GetFileAttributesA(path.c_str()) != INVALID_FILE_ATTRIBUTES;
#else
    struct stat buf;
    return stat(path.c_str(), &buf) == 0;
#endif
}

bool mkdir(const std::string& dir)
{
    std::string parent = dirname(dir);
    if(parent != "." && !exists(parent))
    {
        if(!mkdir(parent))
            return false;
    }
#ifdef _WIN32
    if(!CreateDirectory(dir.c_str(), NULL))
#else
    if(::mkdir(dir.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0)
#endif
        return false;
    return true;
}

std::string executablePath()
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
    return path;
}

std::string testUnit(size_t n)
{
    return n == 1 ? "test" : "tests";
}

std::string testUnitUpper(size_t n)
{
    return n == 1 ? "TEST" : "TESTS";
}

std::string testCaseUnit(size_t n)
{
    return n == 1 ? "test case" : "test cases";
}

bool isDebuggerPresent()
{
#ifdef _WIN32
    return IsDebuggerPresent() != FALSE;
#else
    char buf[2000];
    int fd = open("/proc/self/status", O_RDONLY);
    if (fd == -1)
        return false;
    ssize_t n = read(fd, buf, sizeof(buf) - 1);
    if (n <= 0)
        return false;
    buf[n] = '\0';
    close(fd);
    const char* tracerPid = strstr(buf, "TracerPid:");
    if (!tracerPid)
        return false;
    tracerPid += 10;
    while(isspace(*tracerPid))
        ++tracerPid;
    return isdigit(*tracerPid) && *tracerPid != '0';
#endif
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
    // enable memory checks
#ifdef _WIN32
    #ifdef _DEBUG
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif
#else
    mcheck(NULL);
#endif

    // check if there is a debugger attached
    _debugger = isDebuggerPresent();

    // get test report file name
    std::string outputFile;
    if (outputFile_)
        outputFile = outputFile_;
    else if (char* gunit_output = getenv("GTEST_OUTPUT"))
        outputFile = gunit_output;
    if (!outputFile.empty())
    {
        if (strncmp(outputFile.c_str(), "xml:", 4) != 0 || outputFile.length() == 4)
            outputFile.clear();
        outputFile = outputFile.substr(4);
        if(strchr("/\\", outputFile[outputFile.length() - 1]))
        {
            std::string fileName = basename(executablePath());
            size_t n = fileName.rfind('.');
            if (n != std::string::npos)
                fileName = fileName.substr(0, n);
            fileName += ".xml";
            outputFile += fileName;
        }
    }

    // find tests to run
    std::map<std::string, Suite> activeSuites;
    size_t activeTests = 0;
    for (Test* test = _tests; test; test = test->next)
    {
        if (!filter || match(filter, (std::string(test->suite) + "." + test->name).c_str()))
        {
            TestData testData(test);
            activeSuites[test->suite].tests.push_back(testData);
            ++activeTests;
        }
    }

    // run tests
    std::cout << "[==========] Running " << activeTests << " " << testUnit(activeTests) << " from " << activeSuites.size() << " " << testCaseUnit(activeSuites.size()) << "." << std::endl;
    std::cout << "[----------] Global test environment set-up." << std::endl;

    std::list<std::string> failedTests;
    std::list<std::string> skippedTests;
    int start = time();
    for (std::map<std::string, Suite>::iterator i = activeSuites.begin(), end = activeSuites.end(); i != end; ++i)
    {
        const std::string& suiteName = i->first;
        Suite& suite = i->second;
        std::cout << "[----------] " << suite.tests.size() << " " << testUnit(suite.tests.size()) << " from " << suiteName << std::endl;
        int start = time();
        for (std::list<TestData>::iterator i = suite.tests.begin(), end = suite.tests.end(); i != end; ++i)
        {
            TestData& testData = *i;
            std::cout << "[ RUN      ] " << suiteName << "." << testData.test->name << std::endl;
            _currentTestData = &testData;
            int start = time();
            if (debugger())
                testData.test->func();
            else
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
#ifdef _WIN32
#ifdef _DEBUG
            if (!_CrtCheckMemory())
                fail(testData.test->file, testData.test->line, "detected memory corruption");
#endif
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
            else if(testData.skipped)
            {
                std::cout << "[  SKIPPED ] " << suiteName << "." << testData.test->name << " (" << testData.duration << " ms)" << std::endl;
                skippedTests.push_back(suiteName + "." + testData.test->name);
                ++suite.skipped;
            }
            else
                std::cout << "[       OK ] " << suiteName << "." << testData.test->name << " (" << testData.duration << " ms)" << std::endl;
        }
        suite.duration = time() - start;
        std::cout << "[----------] " << suite.tests.size() << " " << testUnit(suite.tests.size()) << " from " << suiteName << " (" << suite.duration << " ms total)" << std::endl << std::endl;
    }
    int duration = time() - start;

    std::cout << "[----------] Global test environment tear-down" << std::endl;
    std::cout << "[==========] " << activeTests << " " << testUnit(activeTests) << " from " << activeSuites.size() << " " << testCaseUnit(activeSuites.size()) << " ran. (" << duration << " ms total)" << std::endl;

    size_t passedCount = activeTests - failedTests.size() - skippedTests.size();
    std::cout << "[  PASSED  ] " << passedCount << " " << testUnit(passedCount) << "." << std::endl;

    if (!failedTests.empty())
    {
        std::cout << "[  FAILED  ] " << failedTests.size() << " " << testUnit(failedTests.size()) << ", listed below:" << std::endl;
        for (std::list<std::string>::iterator i = failedTests.begin(), end = failedTests.end(); i != end; ++i)
            std::cout << "[  FAILED  ] " << *i << std::endl;
        std::cout << std::endl << failedTests.size() << " FAILED " << testUnitUpper(failedTests.size()) << std::endl;
    }

    // generate test report
    if(!outputFile.empty())
    {
        mkdir(dirname(outputFile));
        std::ofstream file;
        file.open(outputFile.c_str());
        file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        char date[64];
        time_t now = time(0);
        struct tm* time = localtime(&now);
        strftime(date, sizeof(date), "%Y-%m-%dT%H:%M:%S", time);
        file << "<testsuites tests=\"" << activeTests << "\" failures=\"" << failedTests.size() << "\" disabled=\"0\" errors=\"0\" skipped=\"" << skippedTests.size() << "\" timestamp=\"" << date << "\" time=\"" << (duration / 1000.0) << "\" name=\"AllTests\">" << std::endl;
        for (std::map<std::string, Suite>::iterator i = activeSuites.begin(), end = activeSuites.end(); i != end; ++i)
        {
            const std::string& suiteName = i->first;
            Suite& suite = i->second;
            file << "<testsuite name=\"" << suiteName << "\" tests=\"" << suite.tests.size() << "\" failures=\"" << suite.failures << "\" disabled=\"0\" errors=\"0\" skipped=\"" << suite.skipped << "\" time=\"" << (suite.duration / 1000.0) << "\">" << std::endl;
            for (std::list<TestData>::iterator i = suite.tests.begin(), end = suite.tests.end(); i != end; ++i)
            {
                TestData& testData = *i;
                file << "<testcase name=\"" << testData.test->name << "\" status=\"run\" time=\"" << (testData.duration / 1000.0) << "\" classname=\"" << suiteName << "\">" << std::endl;
                for (std::list<std::string>::iterator i = testData.failures.begin(), end = testData.failures.end(); i != end; ++i)
                {
                    const std::string& failure = *i;
                    file << "<failure message=\"" << failure << "\" type=\"\"><![CDATA[" << failure << "]]></failure>" << std::endl;
                }
                if (testData.skipped)
                    file << "<skipped/>" << std::endl;
                file << "</testcase>" << std::endl;
            }
            file << "</testsuite>" << std::endl;
        }
        file << "</testsuites>" << std::endl;
        file.close();
        if (file.fail())
        {
            fprintf(stderr, "Could not create test report '%s'\n", outputFile.c_str());
            return EXIT_FAILURE;
        }
    }

    return failedTests.empty() ? EXIT_SUCCESS : EXIT_FAILURE;
}

void add(Test& test)
{
    test.next = _tests;
    _tests = &test;
}

void fail(const char* file, int line, const char* message)
{
    std::stringstream error;
#ifdef _MSC_VER
    error << file << "(" << line << "): error: " << message ;
#else
    error << file << ":" << line << " error: " << message;
#endif
    std::cerr << error.str() << std::endl;
    if (_currentTestData)
        _currentTestData->failures.push_back(error.str());
}

void skip()
{
    if (_currentTestData)
        _currentTestData->skipped = true;
}

bool debugger()
{
    return _debugger;
}

}
