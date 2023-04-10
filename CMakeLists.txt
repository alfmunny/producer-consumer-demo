cmake_minimum_required(VERSION 3.10)

project(procon)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O0 -ggdb -std=c++20")

include_directories(./include)
link_directories(./lib)
#include_directories(/user/include)
#link_directories(/user/lib/x86_64-linux-gnu)

find_library(PTHREAD pthread)
find_package(Boost)

if (Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS:\"${Boost_INCLUDE_DIRS}\", ENABLE_BOOST is opened")
    message(STATUS "Boost_LIBRARIES:\"${Boost_LIBRARIES}\"")
    message(STATUS "Boost_VERSION:\"${Boost_VERSION}\"")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)

set(SRC
    src/producer.cpp
    src/consumer.cpp
    )

add_library(procon SHARED ${SRC})

set(LIBS
    pthread
    procon
    )

add_executable(demo_lock demo_lock.cpp)
add_dependencies(demo_lock procon)
target_link_libraries(demo_lock ${LIBS})

add_executable(demo_lockfree demo_lockfree.cpp)
add_dependencies(demo_lockfree procon)
target_link_libraries(demo_lockfree ${LIBS})


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

