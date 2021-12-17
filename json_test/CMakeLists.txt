cmake_minimum_required(VERSION 3.5)

project(json_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




add_executable(json_test
    chrono.h
    main.cpp
    )

target_include_directories(json_test PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/json/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include"
    )
