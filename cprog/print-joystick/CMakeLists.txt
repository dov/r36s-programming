cmake_minimum_required(VERSION 3.13)

project(hello-world VERSION 1.0.0 LANGUAGES C)

add_executable(print-joystick
  print-joystick.c
)

find_package(SDL2 REQUIRED)
target_link_libraries(print-joystick PRIVATE SDL2)

