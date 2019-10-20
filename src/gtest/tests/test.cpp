
#include <gtest/gtest.h>

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <algorithm>

TEST(EXPECT_TRUE, true)
{
    EXPECT_TRUE(true);
}

TEST(EXPECT_TRUE, false)
{
    EXPECT_TRUE(false);
}

TEST(EXPECT_FALSE, true)
{
    EXPECT_FALSE(false);
}

TEST(EXPECT_FALSE, false)
{
    EXPECT_FALSE(true);
}

TEST(EXPECT_EQ, true)
{
    EXPECT_EQ(42, 42);
}

TEST(EXPECT_EQ, false)
{
    EXPECT_EQ(42, 0);
}

TEST(EXPECT_NE, true)
{
    EXPECT_NE(42, 0);
}

TEST(EXPECT_NE, false)
{
    EXPECT_NE(42, 42);
}

TEST(EXPECT_GE, true)
{
    EXPECT_GE(43, 42);
    EXPECT_GE(42, 42);
}

TEST(EXPECT_GE, false)
{
    EXPECT_GE(42, 43);
}

TEST(EXPECT_GT, true)
{
    EXPECT_GT(43, 42);
}

TEST(EXPECT_GT, false)
{
    EXPECT_GT(42, 42);
}

TEST(EXPECT_LE, true)
{
    EXPECT_LE(42, 43);
    EXPECT_LE(42, 42);
}

TEST(EXPECT_LE, false)
{
    EXPECT_LE(43, 42);
}

TEST(EXPECT_LT, true)
{
    EXPECT_LT(42, 43);
}

TEST(EXPECT_LT, false)
{
    EXPECT_LT(42, 42);
}

TEST(EXPECT_THROW, true)
{
    int a = 42;
    EXPECT_THROW(throw a, int);
}

TEST(EXPECT_THROW, false)
{
    int a = 42;
    EXPECT_THROW(a = 43, int);
}

TEST(EXPECT_NO_THROW, true)
{
    int a = 42;
    EXPECT_NO_THROW(a = 43);
}

TEST(EXPECT_NO_THROW, false)
{
    int a = 42;
    EXPECT_NO_THROW(throw a);
}

TEST(EXPECT_ANY_THROW, true)
{
    int a = 42;
    EXPECT_ANY_THROW(throw a);
}

TEST(EXPECT_ANY_THROW, false)
{
    int a = 42;
    EXPECT_ANY_THROW(a = 43);
}

TEST(ASSERT_TRUE, true)
{
    ASSERT_TRUE(true);
}

TEST(ASSERT_TRUE, false)
{
    ASSERT_TRUE(false);
}

TEST(ASSERT_FALSE, true)
{
    ASSERT_FALSE(false);
}

TEST(ASSERT_FALSE, false)
{
    ASSERT_FALSE(true);
}

TEST(ASSERT_EQ, true)
{
    ASSERT_EQ(42, 42);
}

TEST(ASSERT_EQ, false)
{
    ASSERT_EQ(42, 0);
}

TEST(ASSERT_NE, true)
{
    ASSERT_NE(42, 0);
}

TEST(ASSERT_NE, false)
{
    ASSERT_NE(42, 42);
}

TEST(ASSERT_GE, true)
{
    ASSERT_GE(43, 42);
    ASSERT_GE(42, 42);
}

TEST(ASSERT_GE, false)
{
    ASSERT_GE(42, 43);
}

TEST(ASSERT_GT, true)
{
    ASSERT_GT(43, 42);
}

TEST(ASSERT_GT, false)
{
    ASSERT_GT(42, 42);
}

TEST(ASSERT_LE, true)
{
    ASSERT_LE(42, 43);
    ASSERT_LE(42, 42);
}

TEST(ASSERT_LE, false)
{
    ASSERT_LE(43, 42);
}

TEST(ASSERT_LT, true)
{
    ASSERT_LT(42, 43);
}

TEST(ASSERT_LT, false)
{
    ASSERT_LT(42, 42);
}

TEST(ASSERT_THROW, true)
{
    int a = 42;
    ASSERT_THROW(throw a, int);
}

TEST(ASSERT_THROW, false)
{
    int a = 42;
    ASSERT_THROW(a = 43, int);
}

TEST(ASSERT_NO_THROW, true)
{
    int a = 42;
    ASSERT_NO_THROW(a = 43);
}

TEST(ASSERT_NO_THROW, false)
{
    int a = 42;
    ASSERT_NO_THROW(throw a);
}

TEST(ASSERT_ANY_THROW, true)
{
    int a = 42;
    ASSERT_ANY_THROW(throw a);
}

TEST(ASSERT_ANY_THROW, false)
{
    int a = 42;
    ASSERT_ANY_THROW(a = 43);
}

TEST(SKIP, skip)
{
    GTEST_SKIP();
    exit(EXIT_FAILURE); // should not be executed
}

TEST(FAIL, fail)
{
    FAIL();
    exit(EXIT_FAILURE); // should not be executed
}

TEST(TestReport, success)
{
}

TEST(TestReport, skip)
{
    GTEST_SKIP();
}

TEST(TestReport, fail)
{
    FAIL();
}

namespace mingtest {

int run(const char* filter, const char* outputFile);

}

int main()
{
    if (!(mingtest::run("EXPECT_TRUE.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_TRUE.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_FALSE.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_FALSE.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_EQ.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_EQ.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_NE.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_NE.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_GE.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_GE.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_GT.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_GT.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_LE.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_LE.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_LT.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_LT.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_THROW.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_THROW.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_NO_THROW.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_NO_THROW.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_ANY_THROW.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_ANY_THROW.false", 0) != 0))
        return EXIT_FAILURE;

    if (!(mingtest::run("ASSERT_TRUE.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_TRUE.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_FALSE.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_FALSE.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_EQ.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_EQ.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_NE.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_NE.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_GE.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_GE.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_GT.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_GT.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_LE.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_LE.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_LT.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_LT.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_THROW.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_THROW.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_NO_THROW.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_NO_THROW.false", 0) != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_ANY_THROW.true", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("ASSERT_ANY_THROW.false", 0) != 0))
        return EXIT_FAILURE;

    if (!(mingtest::run("SKIP.skip", 0) == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("FAIL.fail", 0) != 0))
        return EXIT_FAILURE;

    if (!(mingtest::run("TestReport.*", "xml:testreport.xml") != 0))
        return EXIT_FAILURE;
    {
        {
            std::ifstream testReportFile("testreport.xml");
            std::stringstream buffer;
            buffer << testReportFile.rdbuf();
            std::string testReport = buffer.str();
            testReport.erase(std::remove(testReport.begin(), testReport.end(), '\r'), testReport.end());
            testReport.erase(std::remove(testReport.begin(), testReport.end(), '\n'), testReport.end());
            struct _ {
                static void replace(const std::string& search, const std::string& end, const std::string& value, std::string& str)
                {
                    for (size_t x = 0;;)
                    {
                        size_t a = str.find(search, x);
                        if (a == std::string::npos)
                            break;
                        a += search.length();
                        size_t b = str.find(end, a);
                        str.replace(a, b - a, value);
                        x = a + value.length() + end.length();
                    }
                }
            };
            _::replace("timestamp=\"", "\"", "<timestamp>", testReport);
            _::replace("time=\"", "\"", "<time>", testReport);
            _::replace("message=\"", "\"", "<message>", testReport);
            std::string checkStr = 
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<testsuites name=\"AllTests\" tests=\"3\" failures=\"1\" errors=\"0\" skipped=\"1\" timestamp=\"<timestamp>\" time=\"<time>\">"
"<testsuite name=\"TestReport\" tests=\"3\" failures=\"1\" errors=\"0\" skipped=\"1\" time=\"<time>\">"
"<testcase name=\"success\" time=\"<time>\" classname=\"TestReport\"></testcase>"
"<testcase name=\"skip\" time=\"<time>\" classname=\"TestReport\">"
"<skipped/>"
"</testcase>"
"<testcase name=\"fail\" time=\"<time>\" classname=\"TestReport\">"
"<failure message=\"<message>\" type=\"\"/>"
"</testcase>"
"</testsuite>"
"</testsuites>";
            if (testReport != checkStr)
                return EXIT_FAILURE;
        }
        std::remove("testreport.xml");
    }

    return EXIT_SUCCESS;
}
