#!/bin/sh

./build.sh

cd build/

ctest -j4
