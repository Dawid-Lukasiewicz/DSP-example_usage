cmake_minimum_required(VERSION 3.5)
project(dsp VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)

add_executable(dsp
    main.c
    dtmf.h
    dtmf.c
    tinywav/tinywav.h
    tinywav/tinywav.c
)

add_subdirectory(fft)
link_directories(fft)

# add_subdirectory(tinywav)
# link_directories(tinywav)

if(LINUX_IMPL EQUAL 1)
    message(LINUX_IMPL="${LINUX_IMPL}")
    add_compile_definitions(LINUX_IMPL=1)
    target_link_libraries(dsp fftw3 fft)
else()
    target_link_libraries(dsp fft)
endif()
