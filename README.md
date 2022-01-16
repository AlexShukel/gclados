# GcLaDOS

> Simple testing framework for the C language

The API as well as CLI design of this project was highly inspired by [Jest](https://github.com/facebook/jest#readme).

## Getting started

For building instructions see [BUILD.md](./BUILD.md) file.

### Writing your first test

Create a new file. It's recommended to add `.test.c` extension for file. Write first test case:

```c
#include "gclados.h"

TEST(test1) {
    
}
```

This is first test function. Macro `TEST` required for GcLaDOS to determine, which function is test, and which is not.
Word `test1` is test identifier. It should be unique, as all functions in C.

You can write comment above test case to make more detailed description for your test case. For example:

```c
#include "gclados.h"

// My first test
TEST(test1) {
    
}
```

File could contain multiple test functions, but each should have unique identifier, for example:

```c
#include "gclados.h"

// My first test
TEST(test1) {
    
}

// My second test
TEST(test2) {
    
}
```

Next, add statements to your tests. For this example, we will test function
`sum` which adds two floats.

```c
#include <float.h>

#include "gclados.h"
#include "sum.h"

// Should add two values
TEST(shouldAddValues) {
    ensure(sum(1f, 2f), gclados.toEqualFloat(3f, FLT_EPSILON));
}

```

`ensure` macro receives 2 arguments - `value` and `predicate`.

* `value` - Any value you want to test.
* `predicate` - Function, which will test specified value.

In the example above, we've used `toEqualFloat` predicate. This predicate receives 2 arguments - `value` and `precision`
. `precision` is required, because of rounding. We've specified special precision value `FLT_EPSILON`
from the `<float.h>` file.

Congratulations! You've written your first test.

### Running your first test

There is three options on how you could run your tests. Each option gives the same result, but suits for different
cases. Let's look at them.

#### Standard way

The standard way to run your tests, is to use GcLaDOS cli and `run` command. This method is suitable for most cases, if
you're using GCC compiler.

Simply type:

```commandline
gclados run *.test.c
```

To run all your tests. See `gclados help run` for more information about run command.

#### Using another compiler

If you're not using GCC, but still want automatic entrypoint generation, you could use GcLaDOS cli and `generate`
command.

Run:

```commandline
gclados generate *.test.c --output entry.test.c
```

This command will analyse all files which are ending with ".test.c"
extension, and will create entrypoint "entry.test.c". After that, you can compile this entrypoint with preferred
compiler and run the executable.

#### Manually running tests

The last and least convenient method - running tests manually. This method should be used only if you want to group your
tests differently or if standard entrypoint generation doesn't work for you.

Just create entry.test.c file:

```c
#include <stdbool.h>

#include "gclados.h"
#include "test1.c"

int main() {
    gcladosColors.setColorsSupported(true);
    gcladosSetUpdateSnapshots(false);
    GcladosTest gcladosTests0[] = {
        __gcladosTest_test1("test1"), // pass test description
        __gcladosTest_test2("test2"),
    };
    GcladosTestSuite gcladosTestSuite0 = gcladosCreateTestSuite("<suite name>", gcladosTests0, 2);
    GcladosTestSuite gcladosTestSuites[] = {
        gcladosTestSuite0,
    };
    return gcladosRunTestSuites(gcladosTestSuites, 1);
}
```

Then, you can compile it with any compiler, and run the executable.

The output of these three different methods will be the same, so pick most suitable for your case.

