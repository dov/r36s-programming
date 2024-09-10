cmake_minimum_required(VERSION 3.13)

project(hello-world VERSION 1.0.0 LANGUAGES C)

add_executable(hello-sdl2
  hello-sdl2.c
)

find_package(SDL2 REQUIRED)
target_link_libraries(hello-sdl2 PRIVATE SDL2)

