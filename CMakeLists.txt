cmake_minimum_required(VERSION 3.15)
project(TextEditor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" ON)

# Основной проект
add_executable(text_editor
    src/main.cpp
    src/editor.cpp
    src/file_io.cpp
)

# Документация (используем существующий Doxyfile)
if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    if(DOXYGEN_FOUND)
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating documentation with Doxygen"
            VERBATIM
        )
    endif()
endif()

# Тесты
if(BUILD_TESTS)
    add_executable(tests
        src/tests.cpp
        src/editor.cpp
        src/file_io.cpp
    )
    target_compile_definitions(tests PRIVATE DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)
    enable_testing()
    add_test(NAME tests COMMAND tests)
endif()