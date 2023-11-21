#!/bin/bash

BUILD_PATH=build-manual

pushd ${BUILD_PATH}
./dsp
popd

# python DrawGraphs.py signal_file.csv
# python DrawGraphs.py windowed_signal_file.csv
# python DrawGraphs.py hanning_file.csv
# python DrawGraphs.py bartlett_file.csv
python DrawGraphs.py magnitude_file.csv
