#!/bin/bash

rm *.o test
cp test.src test.cpp
c++ -c SimpleShell.cpp
c++ -c test.cpp
rm test.cpp
c++ test.o SimpleShell.o -o test
