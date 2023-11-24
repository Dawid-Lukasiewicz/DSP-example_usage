cmake_minimum_required(VERSION 3.5)
project(dsp VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)

add_library(fft
    DSPF_sp_cfftr2_dit.h
    DSPF_sp_cfftr2_dit.c
    utility.h
    utility.c
)

target_link_libraries(fft m)
