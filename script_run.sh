#!/bin/bash

BUILD_PATH=build-manual

pushd ${BUILD_PATH}
./dsp
popd

python DrawGraphs.py
