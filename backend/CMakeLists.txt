cmake_minimum_required(VERSION 3.22)
project(optiker LANGUAGES CXX)



if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_STANDARD 20)


# Enable Warnings
set(WARNING_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -Werror")

set(CMAKE_CXX_FLAGS "-pthread")

# Release Flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS}  ${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  ${WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -O3")

# Debug Flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")

include_directories(third_party)
include_directories(src)
include_directories(src/networking)
include_directories(/usr/local/cuda-11.8/include/)

include_directories(${CMAKE_CURRENT_BINARY_DIR}

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/events.pb.cc ${CMAKE_CURRENT_BINARY_DIR}/events.pb.h
    COMMAND protoc --cpp_out=. ${CMAKE_SOURCE_DIR}/../shared/events.proto --proto_path ${CMAKE_SOURCE_DIR}/../shared
    DEPENDS ../interface/datastream.proto
)


set(CORE_SOURCES
    src/main.cpp
    data
    )


add_executable(main
    ${CORE_SOURCES}
    )
    
