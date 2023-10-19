cmake_minimum_required(VERSION 3.5)
project(dsp VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)

add_subdirectory(fft)

add_executable(dsp main.c)

link_directories(libfft)


if(LINUX_IMPL EQUAL 1)
    message(LINUX_IMPL="${LINUX_IMPL}")
    add_compile_definitions(LINUX_IMPL=1)
    target_link_libraries(dsp fftw3 libfft)
else()
    target_link_libraries(dsp libfft)
endif()
