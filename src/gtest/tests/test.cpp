
#include <gtest/gtest.h>

#include <cstdlib>

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
    EXPECT_ANY_THROW(a = 43, int);
}

TEST(FAIL, fail)
{
    FAIL();
}

namespace mingtest {

int run(const char* filter);

}

int main(int argc, const char* argv[])
{
    if (!(mingtest::run("EXPECT_TRUE.true") == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_TRUE.false") != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_FALSE.true") == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_FALSE.false") != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_EQ.true") == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_EQ.false") != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_NE.true") == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_NE.false") != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_GE.true") == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_GE.false") != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_GT.true") == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_GT.false") != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_LE.true") == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_LE.false") != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_LT.true") == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_LT.false") != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_THROW.true") == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_THROW.false") != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_NO_THROW.true") == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_NO_THROW.false") != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_ANY_THROW.true") == 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("EXPECT_ANY_THROW.false") != 0))
        return EXIT_FAILURE;
    if (!(mingtest::run("FAIL.fail") != 0))
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
