cmake_minimum_required(VERSION 3.13)
project(n00m C)

set(CMAKE_C_STANDARD 99)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/executable)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/obj)

# Collect source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.c")

# External libs
find_package(PkgConfig REQUIRED)
pkg_check_modules(CGLM       REQUIRED cglm)
pkg_check_modules(SDL2       REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF   REQUIRED SDL2_ttf)

# Build executable
add_executable(main
  ${SRC_FILES}
  ${CMAKE_SOURCE_DIR}/lib/log.c/src/log.c
  ${CMAKE_SOURCE_DIR}/lib/ini/src/ini.c
)

target_include_directories(main PRIVATE
  ${CGLM_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
  ${SDL2_IMAGE_INCLUDE_DIRS}
  ${SDL2_TTF_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/lib/log.c/src/
  ${CMAKE_SOURCE_DIR}/lib/ini/src/
)

target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic -Wno-missing-braces -O2 -lm)
target_compile_definitions(main PRIVATE LOG_USE_COLOR)

target_link_libraries(main
  ${CGLM_LIBRARIES}
  ${SDL2_LIBRARIES}
  ${SDL2_IMAGE_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
)

