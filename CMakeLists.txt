cmake_minimum_required(VERSION 3.13)
project(project-name C)

set(CMAKE_C_STANDARD 11)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/executable)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/obj)

# Collect source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.c")

# Include and compile the logging library
include_directories(${CMAKE_SOURCE_DIR}/lib/log.c/src/)
add_compile_definitions(LOG_USE_COLOR)
set(SRC_FILES ${SRC_FILES} ${CMAKE_SOURCE_DIR}/lib/log.c/src/log.c)

# Include and compile the INI library
include_directories(${CMAKE_SOURCE_DIR}/lib/ini/src/)
set(SRC_FILES ${SRC_FILES} ${CMAKE_SOURCE_DIR}/lib/ini/src/ini.c)

# Include and compile GLAD
include_directories(${CMAKE_SOURCE_DIR}/lib/glad/include)
set(SRC_FILES ${SRC_FILES} ${CMAKE_SOURCE_DIR}/lib/glad/src/gl.c)

# External libs
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(CGLM REQUIRED cglm)

include_directories(${GLFW_INCLUDE_DIRS} ${CGLM_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS} ${CGLM_LIBRARY_DIRS})

# Build executable
add_executable(main ${SRC_FILES})
target_link_libraries(main ${GLFW_LIBRARIES} ${CGLM_LIBRARIES})

