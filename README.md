
# Ming Test

Ming Test is a minimalistic C++ unit test framework and a "drop in" replacement for the gtest (and gtest_main) library.

Features:
* Very small include header that does not include system or standard library headers
* Easily integratable in CMake projects
* No dependency on C++11
* JUnit test report generation
* Memory checking based on _CrtSetDbgFlag/mcheck
* Does not catch unexpected exceptions when launched in a debugger

## Example

Write C++ code like this:

```cpp
#include <gtest/gtest.h>

TEST(Example, TestName)
{
    int a = 32;
    int b = 32;
    EXPECT_TRUE(a == b);
}
```

... and build it as an executable. Link it against `gtest` and `gtest_main`. Then launch the executable to run the test.
