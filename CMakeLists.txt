# Get CMake PreDefined Variables
# https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html This CMake
# File for OpenGL Projects is compiled by Kushagra I do not claim that this is
# the best CMake Configuration you will get but it does work.

cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(OpenGLStarter)

# tell cmake to generate compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add all Source Files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Find the Required Packages (if Any -> OpenGL/Vulkan or Any)
find_package(OpenGL REQUIRED)

# Variable for the Libs to add to the Linkers
if(WIN32)
  set(LIBS opengl32)
elseif(UNIX)
  set(LIBS GL)
endif()

# use wayland for creating window
set(GLFW_USE_WAYLAND
    ON
    CACHE BOOL "" FORCE)

# Add the External Libraries / Files Directory
add_subdirectory(external)

configure_file(include/cmake_variable.h.in include/cmake_variable.h @ONLY)

target_include_directories(
  ${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include
                          ${PROJECT_BINARY_DIR}/include)

# Include Files from Outside the Workspace Folder -> Like Vulkan SDK

# target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS} )

# Add the Libs to the Linker for the Project

target_link_libraries(${PROJECT_NAME} ${LIBS})

# EXPLANATION FOR SOME STUFF

# GLOB_RECURSE -> Recursively go through all subdirectories of the given path
# and get all the files matching the extension provided file() -> Store the
# Files matching the RegEx into a variable set() -> Alternative to file() if you
# want to set variables for a custom behaviour
