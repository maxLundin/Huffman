cmake_minimum_required(VERSION 3.1)
project(Haffman)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(CMAKE_CXX_STANDARD 11)

add_library(hufflib STATIC lib/ReadWriter.h lib/ReadWriter.cpp lib/bitSeq.h lib/bitSeq.cpp lib/Tree.h lib/Tree.cpp)

add_executable(Haffman main.cpp
#        foo.cpp
        )

target_link_libraries(Haffman hufflib)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14 -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif ()