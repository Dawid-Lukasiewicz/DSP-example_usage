#!/bin/bash

BUILD_PATH=build-manual

cmake -G "Unix Makefiles" -B ./${BUILD_PATH}  -D CMAKE_BUILD_TYPE=Debug

pushd ./${BUILD_PATH}

make dsp

popd