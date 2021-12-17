cmake_minimum_required(VERSION 3.5)

project(lexer_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(lexer_test
    main.cpp
    lexertk/lexertk.hpp
    chrono.h
    code_editor_parser.h
    )
