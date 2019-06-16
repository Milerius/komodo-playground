macro(init_env)
    include(CheckIPOSupported)
    if (UNIX AND NOT APPLE)
        set(LINUX TRUE)
    endif ()
    if (APPLE)
        ##! For user that's install llvm through brew.
        link_directories("/usr/local/opt/llvm/lib")
    endif ()
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(nephtys_compiler clang)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(nephtys_compiler gcc)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(nephtys_compiler msvc)
    endif ()
endmacro()

macro(target_enable_asan target)
    if (USE_ASAN)
        message("-- ASAN Enabled, Configuring...")
        target_compile_options(${target} INTERFACE
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=address -fno-omit-frame-pointer>)
        target_link_options(${target} INTERFACE
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=address -fno-omit-frame-pointer>)
    endif ()
endmacro()


macro(target_enable_tsan target)
    if (USE_TSAN AND NOT ASAN)
        message("-- TSAN Enabled, Configuring...")
        target_compile_options(${target} INTERFACE
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=thread -fno-omit-frame-pointer>)
        target_link_options(${target} INTERFACE
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=thread -fno-omit-frame-pointer>)
    endif ()
endmacro()

macro(target_enable_ubsan target)
    if (USE_UBSAN AND NOT ASAN)
        message("-- UBSAN Enabled, Configuring...")
        target_compile_options(${target} INTERFACE
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=undefined -fno-omit-frame-pointer>)
        target_link_options(${target} INTERFACE
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fsanitize=undefined -fno-omit-frame-pointer>)
    endif ()
endmacro()

macro(target_enable_coverage target)
    if (ENABLE_COVERAGE)
        target_compile_options(${target} PUBLIC $<$<CXX_COMPILER_ID:GNU>:--coverage> -fprofile-arcs -ftest-coverage)
        target_link_options(${target} PUBLIC $<$<CXX_COMPILER_ID:GNU>:--coverage> -fprofile-arcs -ftest-coverage)
    endif ()
endmacro()

macro(target_ipo tgt)
    if (ENABLE_IPO_OPTIMISATION)
        check_ipo_supported(RESULT result OUTPUT output)
        if (result)
            message(STATUS "IPO / LTO enabled")
            set_target_properties(${tgt} PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
        else ()
            message(STATUS "IPO / LTO not supported: <${output}>")
        endif ()
    endif ()
endmacro()

macro(init_common_targets)
    add_library(warning_settings INTERFACE)

    # Using namespaces causes CMake to error our in case of typos on the
    # consuming side, very important.
    add_library(komodo::warning_settings ALIAS warning_settings)

    target_compile_options(
            warning_settings
            INTERFACE
            $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wfatal-errors>
            $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wfatal-errors>
            $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->)

    add_library(defaults_features INTERFACE)
    add_library(komodo::defaults_features ALIAS defaults_features)
    target_compile_features(defaults_features INTERFACE cxx_std_17)

    add_library(optimize_settings INTERFACE)
    add_library(komodo::optimize_settings ALIAS optimize_settings)

    ##! Msvc flags info
    # /Zi - Produces a program database (PDB) that contains type information and symbolic debugging information for use with the debugger.
    # /FS - Allows multiple cl.exe processes to write to the same .pdb file
    # /DEBUG - Enable debug during linking
    # /Od - Disables optimization
    # /Ox - Full optimization
    # /Oy- do not suppress frame pointers (recommended for debugging)

    target_compile_options(optimize_settings INTERFACE
            $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-O2 -march=native>
            $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:-O2 -march=native -pipe>
            $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:-O0 -g -pipe>
            $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>>:-O0 -g>
            $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Zi /FS /DEBUG /Od /MP /MDd /Oy->
            $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/O2 -DNDEBUG /MP>
            )

    add_library(default_settings INTERFACE)
    add_library(komodo::default_settings ALIAS default_settings)
    target_link_libraries(default_settings INTERFACE komodo::warning_settings komodo::optimize_settings komodo::defaults_features)
endmacro()

init_env()
init_common_targets()