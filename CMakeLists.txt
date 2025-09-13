cmake_minimum_required(VERSION 3.21)

project(newleaf_demos
VERSION 0.2.0
HOMEPAGE_URL "https://github.com/kafkaphoenix/newleaf_demos"
DESCRIPTION "Showcase of newleaf engine capabilities"
LANGUAGES CXX
)

message(VERBOSE "*")
message(VERBOSE "* ${PROJECT_NAME} v${PROJECT_VERSION} (${CMAKE_BUILD_TYPE})")
message(VERBOSE "*")

set(TARGET_DEMO "sponza" CACHE STRING "select demo to build")
message(VERBOSE "* Target demo: ${TARGET_DEMO}")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/${TARGET_DEMO}/*.cpp)

set(ICON ${CMAKE_CURRENT_SOURCE_DIR}/packaging/main.rc)

add_executable(${PROJECT_NAME} ${SOURCES} ${ICON})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/${TARGET_DEMO}) # to avoid using relative paths in includes

include(${CMAKE_CURRENT_LIST_DIR}/cmake/warnings.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/copy_assets.cmake)
add_subdirectory(third_party)

set_source_files_properties(${ICON} PROPERTIES LANGUAGE RC)
