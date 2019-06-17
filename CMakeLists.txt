if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. Please create a build directory outside of the source code and call cmake from there")
endif ()

cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(STRINGS "VERSION.md" VERSION_NB)

project(komodo_playground VERSION ${VERSION_NB}
        DESCRIPTION "Open-Source Komodo playground"
        LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(utils)
include(dependencies)

configure_file(config/config.json bin/config.json)

##! Options
option(USE_ASAN "Use address sanitizer" OFF)
option(USE_TSAN "Use thread sanitizer" OFF)
option(USE_UBSAN "Use thread sanitizer" OFF)
option(ENABLE_COVERAGE "Enable coverage for targets" OFF)
option(ENABLE_IPO_OPTIMISATION "Enable IPO optimization" ON)
option(ENABLE_UNIT_TESTS "Enable unit tests for targets" ON)

## Common target for executable / unit tests
add_library(common_dependancies INTERFACE)
add_library(komodo::common_dependancies ALIAS common_dependancies)
target_link_libraries(common_dependancies INTERFACE komodo::default_settings nlohmann_json::nlohmann_json $<$<PLATFORM_ID:Darwin>:c++fs> Replxx::Replxx restclient-cpp)
target_include_directories(common_dependancies INTERFACE extra src)
target_enable_asan(common_dependancies)
target_enable_tsan(common_dependancies)
target_enable_ubsan(common_dependancies)
target_enable_coverage(common_dependancies)
target_ipo(common_dependancies)
target_sources(common_dependancies INTERFACE src/config/config.cpp)


## Our executable / unit tests
add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY bin
        RUNTIME_OUTPUT_DIRECTORY_DEBUG bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE bin)

target_sources(${PROJECT_NAME} PUBLIC src/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC komodo::common_dependancies)

if (ENABLE_UNIT_TESTS)
    add_executable(${PROJECT_NAME}_unit_tests)
    set_target_properties(${PROJECT_NAME}_unit_tests PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY bin
            RUNTIME_OUTPUT_DIRECTORY_DEBUG bin
            RUNTIME_OUTPUT_DIRECTORY_RELEASE bin)
    target_sources(${PROJECT_NAME}_unit_tests PUBLIC src/unit_tests.cpp src/config/config.test.cpp src/shell/executor.test.cpp)
    target_link_libraries(${PROJECT_NAME}_unit_tests doctest komodo::common_dependancies)
endif ()