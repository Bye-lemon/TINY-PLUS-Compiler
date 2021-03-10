cmake_minimum_required(VERSION 3.12)
project(TPC C)

set(CMAKE_C_STANDARD 99)

add_executable(TPC 
    src/main.c src/scan.c src/parse.c src/generate.c src/analyze.c src/utils.c
)

target_include_directories(TPC PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)