
#pragma once

#define TEST(suite, name) \
    void test_##suite##_##name(); \
    struct Test_##suite##_##name { Test_##suite##_##name() {static mingtest::Test test = {#suite, #name, &test_##suite## _##name, __FILE__, __LINE__}; mingtest::add(test);} } _test_##suite##_##name; \
    void test_##suite##_##name()

#define EXPECT_TRUE(e) \
    MINGTEST_ASSERT(e, "EXPECT_TRUE(" #e ") failed")

#define EXPECT_FALSE(e) \
    MINGTEST_ASSERT(!(e), "EXPECT_FALSE(" #e ") failed")

#define EXPECT_EQ(lh, rh) \
    MINGTEST_ASSERT((lh) == (rh), "EXPECT_EQ(" #lh ", " #rh ") failed")

#define EXPECT_NE(lh, rh) \
    MINGTEST_ASSERT((lh) != (rh), "EXPECT_NE(" #lh ", " #rh ") failed")

#define EXPECT_GE(lh, rh) \
    MINGTEST_ASSERT((lh) >= (rh), "EXPECT_GE(" #lh ", " #rh ") failed")

#define EXPECT_GT(lh, rh) \
    MINGTEST_ASSERT((lh) > (rh), "EXPECT_GT(" #lh ", " #rh ") failed")

#define EXPECT_LE(lh, rh) \
    MINGTEST_ASSERT((lh) <= (rh), "EXPECT_LE(" #lh ", " #rh ") failed")

#define EXPECT_LT(lh, rh) \
    MINGTEST_ASSERT((lh) < (rh), "EXPECT_LT(" #lh ", " #rh ") failed")

#define EXPECT_THROW(e, exception) \
    MINGTEST_ASSERT_THROW(e, exception, "EXPECT_THROW(" #e ", " #exception ") failed")

#define EXPECT_NO_THROW(e) \
    MINGTEST_ASSERT_NO_THROW(e, "EXPECT_NO_THROW(" #e ") failed")

#define EXPECT_ANY_THROW(e) \
    MINGTEST_ASSERT_ANY_THROW(e, "EXPECT_ANY_THROW(" #e ") failed")

#define ASSERT_TRUE(e) \
    MINGTEST_ASSERT(e, "ASSERT_TRUE(" #e ") failed")

#define ASSERT_FALSE(e) \
    MINGTEST_ASSERT(!(e), "ASSERT_FALSE(" #e ") failed")

#define ASSERT_EQ(lh, rh) \
    MINGTEST_ASSERT((lh) == (rh), "ASSERT_EQ(" #lh ", " #rh ") failed")

#define ASSERT_NE(lh, rh) \
    MINGTEST_ASSERT((lh) != (rh), "ASSERT_NE(" #lh ", " #rh ") failed")

#define ASSERT_GE(lh, rh) \
    MINGTEST_ASSERT((lh) >= (rh), "ASSERT_GE(" #lh ", " #rh ") failed")

#define ASSERT_GT(lh, rh) \
    MINGTEST_ASSERT((lh) > (rh), "ASSERT_GT(" #lh ", " #rh ") failed")

#define ASSERT_LE(lh, rh) \
    MINGTEST_ASSERT((lh) <= (rh), "ASSERT_LE(" #lh ", " #rh ") failed")

#define ASSERT_LT(lh, rh) \
    MINGTEST_ASSERT((lh) < (rh), "ASSERT_LT(" #lh ", " #rh ") failed")

#define ASSERT_THROW(e, exception) \
    MINGTEST_ASSERT_THROW(e, exception, "ASSERT_THROW(" #e ", " #exception ") failed")

#define ASSERT_NO_THROW(e) \
    MINGTEST_ASSERT_NO_THROW(e, "ASSERT_NO_THROW(" #e ") failed")

#define ASSERT_ANY_THROW(e) \
    MINGTEST_ASSERT_ANY_THROW(e, "ASSERT_ANY_THROW(" #e ") failed")

#define FAIL() \
    MINGTEST_FAIL("FAIL() reached")

#define GTEST_SKIP() \
    MINGTEST_SKIP()

namespace mingtest {

struct Test
{
    const char* suite;
    const char* name;
    void (*func)();
    const char* file;
    int line;
    Test* next;
};

void add(Test& test);
void fail(const char* file, int line, const char* message);
void exit();
void skip();
bool debugger();

}

#define _MINGTEST_FAIL(message) \
    mingtest::fail(__FILE__, __LINE__, message)

#define MINGTEST_FAIL(message) \
    do { _MINGTEST_FAIL(message); mingtest::exit(); } while(false)

#define MINGTEST_SKIP() \
    do { mingtest::skip(); mingtest::exit(); } while(false)

#define MINGTEST_ASSERT(e, message) \
    do { if(!(e)) _MINGTEST_FAIL(message); } while (false)

#define MINGTEST_ASSERT_THROW(e, exception, message) \
    do { \
        if (mingtest::debugger()) { try { e; _MINGTEST_FAIL(message); } catch(exception) { } } \
        else { try { e; _MINGTEST_FAIL(message); } catch(exception) { } catch(...) { _MINGTEST_FAIL(message);} } \
    } while (false)

#define MINGTEST_ASSERT_NO_THROW(e, message) \
    do { \
        if (mingtest::debugger()) { e; } \
        else { try { e; } catch(...) { _MINGTEST_FAIL(message); } } \
    } while (false)

#define MINGTEST_ASSERT_ANY_THROW(e, message) \
    do { try { e; _MINGTEST_FAIL(message); } catch(...) { } } while (false)
