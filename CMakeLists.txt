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

# Obtain the version of project that is being built.
FIND_PACKAGE(Git)
IF(GIT_FOUND)
    EXECUTE_PROCESS(
            COMMAND ${GIT_EXECUTABLE} describe --tags --long --always --dirty=-dirty
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            RESULT_VARIABLE PROC_RESULT
            OUTPUT_VARIABLE FULL_VERSION
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE)

    EXECUTE_PROCESS(
            COMMAND ${GIT_EXECUTABLE} describe --abbrev=0 --tags
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            RESULT_VARIABLE PROC_RESULT
            OUTPUT_VARIABLE LAST_TAG
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE)
ENDIF()
IF(NOT GIT_FOUND OR NOT PROC_RESULT EQUAL 0)
    # If git is not available on the build platform, use this as a fallback.
    SET(LAST_TAG 2.5.9)
    SET(FULL_VERSION ${LAST_TAG}-LOCAL)
ENDIF()
MESSAGE(STATUS "Configuring version ${FULL_VERSION}")

PROJECT(mupen64minus-ui-console LANGUAGES C VERSION 2.5.9)
# Initialise project information.
PROJECT(mupen64minus-ui-console
        LANGUAGES C
        VERSION ${LAST_TAG}
        DESCRIPTION "mupen64minus console front-end"
        HOMEPAGE_URL "https://github.com/deltabeard/mupen64minus-ui-console")
SET(PROJECT_LICENSE "GPL-2.0")

FIND_PACKAGE(Threads)
message(CHECK_START "Checking Thread support")
IF(Threads_FOUND AND CMAKE_USE_PTHREADS_INIT)
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

IF(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND APPLE)
    SET(EXE_TARGET_TYPE MACOSX_BUNDLE)
    MESSAGE(VERBOSE "Setting EXE type to MACOSX Bundle")
ENDIF()
ADD_EXECUTABLE(mupen64minus-ui-console ${EXE_TARGET_TYPE}
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
# Package options
IF(APPLE)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
            MACOSX_BUNDLE_BUNDLE_VERSION ${LAST_TAG}
            MACOSX_BUNDLE_COPYRIGHT ${PROJECT_LICENSE}
            MACOSX_BUNDLE_INFO_STRING ${PROJECT_DESCRIPTION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${FULL_VERSION}
            MACOSX_BUNDLE_LONG_VERSION_STRING ${FULL_VERSION})
    INSTALL(TARGETS mupen64minus-ui-console BUNDLE
        DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME})
ELSE()
    INSTALL(TARGETS mupen64minus-ui-console RUNTIME)
ENDIF()
INSTALL(FILES
		${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME}.pdb
		${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.pdb
	TYPE BIN
	CONFIGURATIONS Debug RelWithDebInfo
	OPTIONAL)
