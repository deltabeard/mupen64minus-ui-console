# vim: ts=4:sw=4:expandtab
CMAKE_MINIMUM_REQUIRED(VERSION 3.18...3.28)

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

PROJECT(mupen64minus-ui-console LANGUAGES C VERSION 2.5.9)

FIND_PACKAGE(Threads)
message(CHECK_START "Checking Thread support")
IF(Threads_FOUND AND (CMAKE_USE_WIN32_THREADS_INIT OR CMAKE_USE_PTHREADS_INIT))
    SET(THREADS_SUPPORTED ON)
    message(CHECK_PASS "supported")
ELSE()
    SET(THREADS_SUPPORTED OFF)
    message(CHECK_FAIL "not supported")
ENDIF()

SET(M64API_DIR ../mupen64plus-core/src/api CACHE PATH "Path to Mupen64 API directory")
FILE(REAL_PATH "${M64API_DIR}" M64API_DIR_fullpath EXPAND_TILDE)
IF(NOT EXISTS "${M64API_DIR_fullpath}")
    MESSAGE(FATAL_ERROR "Mupen64 API directory '${M64API_DIR_fullpath}' does not exist.")
ENDIF()

ADD_EXECUTABLE(mupen64minus-ui-console
    ./src/cheat.c
    ./src/compare_core.c
    ./src/core_interface.c
    ./src/main.c
    ./src/plugin.c)

# Only enable debugger if thread support available.
OPTION(ENABLE_DEBUGGER "Enable M64 debugger" ${THREADS_SUPPORTED})
IF(ENABLE_DEBUGGER)
    TARGET_SOURCES(mupen64minus-ui-console PRIVATE ./src/debugger.c)
    TARGET_COMPILE_DEFINITIONS(mupen64minus-ui-console PRIVATE ENABLE_DEBUGGER=1)
    TARGET_LINK_LIBRARIES(mupen64minus-ui-console ${CMAKE_THREAD_LIBS_INIT})
ELSE()
    TARGET_COMPILE_DEFINITIONS(mupen64minus-ui-console PRIVATE ENABLE_DEBUGGER=0)
    LIST(APPEND m64_disabled_features Debugger)
ENDIF()

IF(WIN32)
    TARGET_SOURCES(mupen64minus-ui-console PRIVATE
        ./src/osal_dynamiclib_win32.c
        ./src/osal_files_win32.c)
ELSE()
    TARGET_SOURCES(mupen64minus-ui-console PRIVATE
        ./src/osal_dynamiclib_unix.c
        ./src/osal_files_unix.c)
    TARGET_LINK_LIBRARIES(mupen64minus-ui-console PRIVATE dl)
ENDIF()

TARGET_INCLUDE_DIRECTORIES(mupen64minus-ui-console PRIVATE ${M64API_DIR})

IF(m64_disabled_features)
    MESSAGE(STATUS "Disabled features: ${m64_disabled_features}")
ENDIF()

INCLUDE(CPack)
INSTALL(TARGETS mupen64minus-ui-console RUNTIME)
