# Polynomial 3-Sat solver
This is a reference implementation of Ortho's polynomial 3-Sat solver (http://polynomial3sat.org). The programing language used is  C++.

# Requirements

This code has been tested in CentOS 7.2 using GCC 7.2 compiler only. However, any compiler with C++17 compatibility should be able to build and run it. There are no special libraries needed  to build this code, other than what comes with a standard C++ library.  CMake is required to build the code.

## Build  

Use provided CmakeLists.txt file with cmake.
```
mkdir build && cd build
cmake ..
make -j
```
##  Mac users

Install g++-7 compiler using brew

```
brew install gcc@7
```

In cmake-gui specify native compilers and point them to
```
/usr/local/bin/g++-7 and /usr/local/bin/gcc-7
```
# Run

Resultant executable (default name is solver) takes one command line argument which is the path to a DIMACs file.

```
./solver myfile.dimcac
```

