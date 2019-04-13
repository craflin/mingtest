
#pragma once

#define TEST(suite, name) \
    void test_##suite##_##name(); \
    struct Test_##suite##_##name { Test_##suite##_##name() {static mingtest::Test test = {#suite, #name, &test_##suite## _##name, __FILE__, __LINE__}; mingtest::add(test);} } _test_##suite##_##name; \
    void test_##suite##_##name()

#define EXPECT_TRUE(e) \
    do { if(!(e)) mingtest::fail(__FILE__, __LINE__, "EXPECT_TRUE(" #e ") failed"); } while (false)

#define EXPECT_FALSE(e) \
    do { if(e) mingtest::fail(__FILE__, __LINE__, "EXPECT_FALSE(" #e ") failed"); } while (false)

#define EXPECT_EQ(lh, rh) \
    do { if(!((lh) == (rh))) mingtest::fail(__FILE__, __LINE__, "EXPECT_EQ(" #lh ", " #rh ") failed"); } while (false)

#define EXPECT_NE(lh, rh) \
    do { if(!((lh) != (rh))) mingtest::fail(__FILE__, __LINE__, "EXPECT_NE(" #lh ", " #rh ") failed"); } while (false)

#define EXPECT_GE(lh, rh) \
    do { if(!((lh) >= (rh))) mingtest::fail(__FILE__, __LINE__, "EXPECT_GE(" #lh ", " #rh ") failed"); } while (false)

#define EXPECT_GT(lh, rh) \
    do { if(!((lh) > (rh))) mingtest::fail(__FILE__, __LINE__, "EXPECT_GT(" #lh ", " #rh ") failed"); } while (false)

#define EXPECT_LE(lh, rh) \
    do { if(!((lh) <= (rh))) mingtest::fail(__FILE__, __LINE__, "EXPECT_LE(" #lh ", " #rh ") failed"); } while (false)

#define EXPECT_LT(lh, rh) \
    do { if(!((lh) < (rh))) mingtest::fail(__FILE__, __LINE__, "EXPECT_LT(" #lh ", " #rh ") failed"); } while (false)

#define EXPECT_THROW(e, exception) \
    do { \
        if (mingtest::debugger()) { try { e; mingtest::fail(__FILE__, __LINE__, "EXPECT_THROW(" #e ") failed"); } catch(exception) { } } \
        else { try { e; mingtest::fail(__FILE__, __LINE__, "EXPECT_THROW(" #e ") failed"); } catch(exception) { } catch(...) { mingtest::fail(__FILE__, __LINE__, "EXPECT_THROW(" #e ")");} } \
    } while (false)

#define EXPECT_NO_THROW(e) \
    do { \
        if (mingtest::debugger()) { e; } \
        else { try { e; } catch(...) { mingtest::fail(__FILE__, __LINE__, "EXPECT_NO_THROW(" #e ") failed"); } } \
    } while (false)

#define EXPECT_ANY_THROW(e) \
    do { try { e; mingtest::fail(__FILE__, __LINE__, "EXPECT_THROW(" #e ") failed"); } catch(...) { } } while (false)

#define FAIL() \
    do { mingtest::fail(__FILE__, __LINE__, "FAIL() reached"); } while(false)

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
bool debugger();

}
