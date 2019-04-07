
#include <gtest/gtest.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <time.h>
#endif

#include <list>
#include <iostream>
#include <string>

namespace {

struct Test
{
    const char* name;
    void (*func)();
};

struct Suite
{
    const char* suite;
    std::list<Test> tests;
};

std::list<Suite>* _suites = 0;
bool _failed = false;

bool match(const std::string& test, const char* filter)
{
    if (test == filter)
        return true;
    return false;
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

}

namespace mingtest {

int run(const char* filter)
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
                if (!filter || match(std::string(suite.suite) + "." + test.name, filter))
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
        std::cout << "[----------] " << suite.tests.size() << " " << Print::testUnit(suite.tests.size()) << " from " << suite.suite << std::endl;
        int start = time();
        for (std::list<Test>::iterator i = suite.tests.begin(), end = suite.tests.end(); i != end; ++i)
        {
            Test& test = *i;
            std::cout << "[ RUN      ] " << suite.suite << "." << test.name << std::endl;
            _failed = false;
            int start = time();
            if (debugger())
            {
                try
                {
                    test.func();
                }
                catch (...)
                {
                    _failed = true;
                }
            }
            else
                test.func();
            int duration = time() - start;
            if (_failed)
            {
                std::cout << "[  FAILED  ] " << suite.suite << "." << test.name << " (" << duration << " ms)" << std::endl;
                failedTests.push_back(std::string(suite.suite) + "." + test.name);
            }
            else
                std::cout << "[       OK ] " << suite.suite << "." << test.name << " (" << duration << " ms)" << std::endl;
        }
        int duration = time() - start;
        std::cout << "[----------] " << suite.tests.size() << " " << Print::testUnit(suite.tests.size()) << " from " << suite.suite << " (" << duration << " ms total)" << std::endl << std::endl;
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
#ifdef _MSC_VER
    std::cerr << file << "(" << line << "): error: " << expression << " failed" << std::endl;
#else
    std::cerr << file << ":" << line << " error: " << expression << " failed" << std::endl;
#endif
    _failed = true;
}

bool debugger()
{
    return false;
}

}
