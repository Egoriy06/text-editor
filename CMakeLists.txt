cmake_minimum_required(VERSION 3.10)
project(TextEditor)

set(CMAKE_CXX_STANDARD 17)

add_executable(text_editor
    src/main.cpp
    src/file_io.cpp
    src/editor.cpp
)