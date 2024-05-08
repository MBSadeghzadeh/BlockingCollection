cmake_minimum_required(VERSION 3.0)

project(ProducerConsumer VERSION 1.0.0 LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(ProducerConsumer src/main.cpp)

target_include_directories(ProducerConsumer PRIVATE ${CMAKE_SOURCE_DIR})

if(TESTS_ENABLED)
    add_subdirectory(tests)
endif(TESTS_ENABLED)