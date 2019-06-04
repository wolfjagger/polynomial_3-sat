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
# Output

Output begins with a start time and then states if pre-processing was completed. 
Otherwise, it is reported if satisfiability or un-satisfiability was discovered during pre-processing.

A valid vertex is a vertex that remained after pre-processing, that's not associated with 
a pure literal. If we assume that a given 3-SAT had no pure literals, quantum clauses, duplicates or clauses of size 1, then the number of vertices is at least twice the number of clauses c plus one, and at most, 3c.


After pre-processing, the edge-sequences (think of edges with additional information, see definition 2.2 of 
the paper found: polynomial3sat.org), are contructed whose endpoints are the valid 
vertices. Also, each vertex has a corresponding vertex-sequence (which are vertices with 
additional information, see definition 2.6). Lastly, the number of S-sets is the number of clauses pairwise, (c choose 2).

Note well that essentially, S-set Comparing is the algorithm (see page 15 of the paper).

The output will report if the 3-SAT is satisfiable or un-satisfiable. 
It will not produce a solution if satisfiable, but other versions do, as described on page 33 of 
the paper. More precisely, if round 1 completes, the 3-SAT has at least one solution. 
ie. the 3-SAT is satisfiable. If round 1 does not complete, the 3-SAT is un-satisfiable.

If round 1 completes, then there are edge and vertex sequences remaining. 
A remaining vertex-sequence corresponds to a literal associated to its vertex, that belongs to a solution. 
For example, suppose there were x distinct literals say, that belong to no solution and there 
were y instances (in y clauses), for each of the x literals, then the number of vertex-sequences 
removed by the algorithm would be xy.

Note that if the number of remaining edge-sequences is less than the number initially 
constructed, but the number of vertex-sequences remains unchanged, it means that every 
literal belongs to at least one solution, but not every pair of literals belong to a solution together.
Also note, that if the 3-SAT is satisfiable, then the number of S-sets must remain unchanged, 
since every S-set contains an edge-sequence for every solution.

Assuming pre-processing was completed, output ends with the time in which round 1 was completed for satisfiability, or the time 
in which un-satisfiability was discovered during round 1. 

