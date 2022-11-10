# ctulu_setup_cmake([DISABLE_SRC_BUILD] [REQUIRE_STANDARD languages...])
# Configure some variables of CMake.
#    {option}  [in] DISABLE_SRC_BUILD:      Set ${CMAKE_DISABLE_SOURCE_CHANGES} and ${CMAKE_DISABLE_IN_SOURCE_BUILD} to ON.
#    {option}  [in] languages:              Disable compiler extension and requires exact standard the given language.
#                                           (C and CXX supported)
function(ctulu_setup_cmake)
    set(options DISABLE_SRC_BUILD)
    set(oneValueArgs)
    set(multiValueArgs REQUIRE_STANDARD)
    cmake_parse_arguments(ctulu_setup_cmake "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(ctulu_setup_cmake_REQUIRE_STANDARD)
        message(STATUS "Ctulu -- Disabling Compiler extension")
    endif()

    foreach(language ${ctulu_setup_cmake_REQUIRE_STANDARD})
        # disable compiler specific extensions
        set(CMAKE_${language}_EXTENSIONS OFF PARENT_SCOPE)
        # set STANDARD as a requirement
        set(CMAKE_${language}_STANDARD_REQUIRED ON PARENT_SCOPE)
    endforeach()

    if (ctulu_setup_cmake_DISABLE_SRC_BUILD)
        message(STATUS "Ctulu -- Disabling build in source")

        # don't allow to build in sources otherwise a makefile not generated by CMake can be overridden
        set(CMAKE_DISABLE_SOURCE_CHANGES ON PARENT_SCOPE)
        set(CMAKE_DISABLE_IN_SOURCE_BUILD ON PARENT_SCOPE)
    endif()
endfunction()