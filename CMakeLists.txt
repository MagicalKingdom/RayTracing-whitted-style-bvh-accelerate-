cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(. SOURCES)

add_executable(${PROJECT_NAME}  ${SOURCES})