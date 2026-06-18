cmake_minimum_required(VERSION 3.16)

project(SDPO_Lab2_SAT_DPLL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SAT_DPLL
    SAT_DPLL/main.cpp
    SAT_DPLL/BBV.cpp
    SAT_DPLL/branchingstrategy.cpp
    SAT_DPLL/boolequation.cpp
    SAT_DPLL/boolinterval.cpp
    Allocator/Allocator.cpp
)

target_include_directories(SAT_DPLL PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/SAT_DPLL
    ${CMAKE_CURRENT_SOURCE_DIR}/Allocator
)

if (MSVC)
    target_compile_options(SAT_DPLL PRIVATE /W4)
else()
    target_compile_options(SAT_DPLL PRIVATE -Wall -Wextra -Wpedantic)
endif()
