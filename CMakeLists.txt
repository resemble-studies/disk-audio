# License: MIT. See LICENSE in root directory.
# Date: 2020/06/29

cmake_minimum_required(VERSION 3.1.0)

project("ensemble-disk-audio" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories("${CMAKE_CURRENT_LIST_DIR}/inc")

set(ENSEMBLE_DISK_AUDIO_LIB "ensemble-disk-audio-lib")
add_library(${ENSEMBLE_DISK_AUDIO_LIB} STATIC "")
target_sources(
    ${ENSEMBLE_DISK_AUDIO_LIB}
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/src/ensemble/disk-audio.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/inc/ensemble/disk-audio.hpp"
)

if (CMAKE_BUILD_TYPE MATCHES "^[Dd]ebug")
    set(ENSEMBLE_DISK_AUDIO_EXE "ensemble-disk-audio-exe")
    add_executable(
        ${ENSEMBLE_DISK_AUDIO_EXE}
        "${CMAKE_CURRENT_LIST_DIR}/test/main.cpp"
    )
    target_link_libraries(
        ${ENSEMBLE_DISK_AUDIO_EXE}
        ${ENSEMBLE_DISK_AUDIO_LIB}
    )
endif ()
