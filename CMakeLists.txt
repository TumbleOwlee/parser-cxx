# Set version requirement
cmake_minimum_required(VERSION 3.25)

# Define project
project(expression
    VERSION 0.0.1
    DESCRIPTION "Draft implementation of an expression evaluation processor."
    LANGUAGES C CXX)

# Export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default to Debug
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()
message("Build type: ${CMAKE_BUILD_TYPE}")

# Create list of all source files
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*" )
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/*" )

# Create executable
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Create list of all example files
file(GLOB EXAMPLES "${CMAKE_CURRENT_SOURCE_DIR}/examples/*" )

# Build examples
foreach(FILE IN LISTS EXAMPLES)
    get_filename_component(EXAMPLE_NAME ${FILE} NAME_WE)
    add_executable(${PROJECT_NAME}-${EXAMPLE_NAME} ${FILE})
    target_include_directories(${PROJECT_NAME}-${EXAMPLE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${PROJECT_NAME}-${EXAMPLE_NAME} ${PROJECT_NAME})
endforeach()
