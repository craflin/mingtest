
# Ming Test

[![Build Status](http://xaws6t1emwa2m5pr.myfritz.net:8080/buildStatus/icon?job=craflin%2Fmingtest%2Fmaster)](http://xaws6t1emwa2m5pr.myfritz.net:8080/job/craflin/job/mingtest/job/master/)

Ming Test is a minimalistic C++ unit test framework and a "drop in" replacement for the gtest (and the gtest_main) library. Contrarily to google test, it compiles quickly, does some basic memory consistency checking, does not depend on modern C++, does not conflict with other libraries, does not cause warnings when evaluating simple expressions and does not interfere with debugging.

Features:
* Very small include header that does not include system or standard library headers
* Easily integratable in CMake projects
* No dependency on C++11
* JUnit test report generation
* Memory checking on Windows based on `_CrtSetDbgFlag`
* No catching of unexpected exceptions when launched in a debugger
* Breaking at failed assertions when running in a debugger

## Example

Write C++ code like this:

```cpp
#include <gtest/gtest.h>

TEST(Example, TestName1)
{
    int a = 32;
    int b = 32;
    EXPECT_TRUE(a == b);
}

TEST(Example, TestName2)
{
    int a = 32;
    int b = 33;
    EXPECT_FALSE(a == b);
}
```

... and build it as an executable. Link it against `gtest` and `gtest_main`. Then launch the executable to run the test. You can also use the assertion macros `EXPECT_THROW` and `EXPECT_ANY_THROW`. (And some rather pointless macros like `EXPECT_EQ`, `EXPECT_NE`, `EXPECT_GE`, `EXPECT_GT`, `EXPECT_LE`, `EXPECT_LT` and `EXPECT_NO_THROW` if you like.)
