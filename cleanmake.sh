#!/bin/bash

#TARGET=${1}
#echo "The target is: $TARGET"
# cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DTARGET_NODEMCU=ON ..

rm -rf build
mkdir build
cd build
cmake -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DTARGET_NODEMCU=ON ..
#make
