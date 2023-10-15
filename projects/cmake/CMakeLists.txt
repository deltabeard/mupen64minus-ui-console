# vim: ts=4:sw=4:expandtab
CMAKE_MINIMUM_REQUIRED(VERSION 3.18...3.23)

## Check user set options.
IF(NOT CMAKE_BUILD_TYPE)
    MESSAGE(STATUS "CMAKE_BUILD_TYPE was not set by user; setting build type to Debug")
    SET(CMAKE_BUILD_TYPE "Debug")
ELSE()
    # List of valid build types
    SET(VALID_BUILD_TYPES Debug Release RelWithDebInfo MinSizeRel)
    LIST(FIND VALID_BUILD_TYPES ${CMAKE_BUILD_TYPE} IS_VALID_BUILD_TYPE)
    IF(IS_VALID_BUILD_TYPE EQUAL -1)
        MESSAGE(FATAL_ERROR "CMAKE_BUILD_TYPE was '${CMAKE_BUILD_TYPE}' but can only be set to one of ${VALID_BUILD_TYPES}")
    ENDIF()
ENDIF()

PROJECT(mupen64minus-ui-console LANGUAGES VERSION 2.5.9)

set(M64API_DIR ../../../mupen64plus-core/src/api CACHE PATH "Path to Mupen64 API folder")
set(SRCDIR ../../src)

ADD_EXECUTABLE(mupen64minus-ui-console
    ${SRCDIR}/cheat.c
    ${SRCDIR}/compare_core.c
    ${SRCDIR}/core_interface.c
    ${SRCDIR}/debugger.c
    ${SRCDIR}/main.c
    ${SRCDIR}/plugin.c)

if(WIN32)
    TARGET_SOURCES(mupen64minus-ui-console PRIVATE
        ${SRCDIR}/osal_dynamiclib_win32.c
        ${SRCDIR}/osal_files_win32.c)
else()
    TARGET_SOURCES(mupen64minus-ui-console PRIVATE
        ${SRCDIR}/osal_dynamiclib_unix.c
        ${SRCDIR}/osal_files_unix.c)
    target_link_libraries(mupen64minus-ui-console PRIVATE dl)
endif()

target_include_directories(mupen64minus-ui-console PRIVATE ${M64API_DIR})
