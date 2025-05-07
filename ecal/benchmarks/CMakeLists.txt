cmake_minimum_required(VERSION 3.15)

project(test_hello_world)

find_package(benchmark REQUIRED)

set(source_files
  test_hello_world.cpp
)

add_executable(${PROJECT_NAME} ${source_files})

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    eCAL::core
    benchmark::benchmark
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)