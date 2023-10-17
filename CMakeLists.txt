cmake_minimum_required(VERSION 3.5)
project(dsp VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)

add_subdirectory(fft)

add_executable(dsp main.c)

link_directories(libfft)

target_link_libraries(dsp libfft)
