cmake_minimum_required(VERSION 3.25)

project(cxx_template)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    #add thread only linux gcc
    set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++17 -pthread")

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Wall -fPIC)
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    #解决 visual studio 编码问题,暂时不支持 windows
    if (WIN32)
        add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    endif ()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

else()
    MESSAGE(FATAL_ERROR "Not Support The OS!")
endif()

include(FetchContent)

FetchContent_Declare(
        googletest   #要获取的依赖项的名称
        GIT_REPOSITORY "git@github.com:google/googletest.git" # Git仓库地址
        GIT_TAG "v1.14.0"  # 指定要检出的标签或分支
        GIT_PROGRESS TRUE
)

FetchContent_Declare(
        benchmark   #要获取的依赖项的名称
        GIT_REPOSITORY "git@github.com:google/benchmark.git" # Git仓库地址
        GIT_TAG "v1.8.4"  # 指定要检出的标签或分支
        GIT_PROGRESS TRUE
)

set(FETCHCONTENT_FULLY_DISCONNECTED ON)

# 初始化外部内容
FetchContent_MakeAvailable(googletest benchmark)

option(NEED_TO_OPEN_TEST "是否启动测试" ON )

if (NEED_TO_OPEN_TEST)
    # 开启测试
    enable_testing()
endif ()

message(STATUS "Download googletest source dir: ${googletest_SOURCE_DIR}")
message(STATUS "Download googletest binary dir: ${googletest_BINARY_DIR}")
message(STATUS "Download benchmark binary dir: ${benchmark_BINARY_DIR}")
message(STATUS "Download benchmark binary dir: ${benchmark_BINARY_DIR}")

set(PROGRAM_NAME "main")

add_executable(${PROGRAM_NAME} main.cpp)

function(INCLUDE_GOOGLE_TEST_BENCHMARK target_name)
    target_include_directories(${target_name} PUBLIC
            ${googletest_SOURCE_DIR}/googletest/include
            ${googletest_SOURCE_DIR}/googlemock/include
            ${benchmark_SOURCE_DIR}/include
            ${benchmark_SOURCE_DIR}/include
    )
endfunction()

target_link_libraries(${PROGRAM_NAME})


