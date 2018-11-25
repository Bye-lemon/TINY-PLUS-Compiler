cmake_minimum_required(VERSION 3.12)
project(TPC C)

set(CMAKE_C_STANDARD 99)

add_executable(TPC main.c globals.h scan.c scan.h utils.c utils.h)