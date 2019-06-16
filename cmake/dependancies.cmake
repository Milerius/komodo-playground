include(FetchContent)

macro(download_dependancies)
    message("Downloading dependancies...")
    set(JSON_MultipleHeaders ON)
    cmake_policy(SET CMP0077 NEW)
    SET(JSON_BuildTests OFF CACHE BOOL "Disable dependancies json tests")
    SET(DOCTEST_WITH_TESTS OFF CACHE BOOL "Disable dependancies doctest tests")
    SET(DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF CACHE BOOL "Disable dependancies doctest static library")

    FetchContent_Declare(
            json
            GIT_REPOSITORY https://github.com/nlohmann/json
            GIT_TAG v3.6.1
    )

    FetchContent_Declare(
            doctest
            GIT_REPOSITORY https://github.com/onqtam/doctest
            GIT_TAG 2.3.3
    )
    FetchContent_MakeAvailable(json doctest)
    message("Successfully downloaded dependancies")
endmacro()

download_dependancies()