# Polynomial 3-Sat solver
This is a reference implementation of Ortho's polynomial 3-Sat solver (http://http://polynomial3sat.org). Programing language used is  C++.

# Requirements

This code has been tested in CentOS 7.2 using GCC 7.2 compiler only. However any compiler with C++17 compatibility should be able to build and run it. There are no special libraries needed, other than the ones come with standard C++ library, to build this code. CMake is required to build the code. 


## Build  

Easiest way to build the code is using CMake. CmakeLists.txt is provided with the code
```
mkdir build && cd build
cmake ..
make -j
```
# Run

Resultant executable (default name is solver) takes one command line argument which is the path to a DIMACs file.

```
./solver myfile.dimcac
```

