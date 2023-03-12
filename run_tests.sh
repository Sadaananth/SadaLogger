#!/bin/sh

./build.sh

cd build/

gtest=0
all=0
if [ ! -z "$@" ]
then
    if [ "$1" = "-g" ]
    then
        gtest=1
    elif [ "$1" = "-a" ]
    then
        all=1
    fi
fi

if [ $all -eq 1 ]
then
    ctest -j4
    cd tests/
    ./LoggerTest
elif [ $gtest -eq 1 ]
then
    ctest -j4
else
    cd tests/
    ./LoggerTest
fi
