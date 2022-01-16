# Building GcLaDOS

Firstly, make sure you've installed all these tools:

1. [CMake](https://cmake.org/) (at least 3.20)
2. [GCC](https://gcc.gnu.org/) (at least 11.2.0)
3. Make, or any other alternative.

## Debug

To build packages for debugging, run these commands one after another:

1). Generate project

```commandline
cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" -S . -Wdev
```

2). Build

```commandline
cmake --build cmake-build-debug -Wdev
```

## Release

To build all packages in release mode, run these commands:

1). Generate project

```commandline
cmake -B cmake-build-release -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -S .
```

2). Build

```commandline
cmake --build cmake-build-release
```

Required files will appear in `bin` directory.