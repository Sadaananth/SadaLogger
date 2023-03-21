#!/bin/sh

./clean.sh
./build.sh

zip SadaLogger-linux-x86_64-release.zip build/libSadaLogger.so src/Logger.hpp
