cmake_minimum_required(VERSION 3.14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX ${CMAKE_CURRENT_SOURCE_DIR}/CMake/cxx_flag_overrides.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_C ${CMAKE_CURRENT_SOURCE_DIR}/CMake/c_flag_overrides.cmake)

project(agssqlite
		VERSION 0.1.0
		DESCRIPTION "AGS Plugin for SQLite usage."
		LANGUAGES CXX C
		)

set(CMAKE_CXX_STANDARD 11)

if(${CMAKE_BUILD_TYPE} MATCHES Release)
	message("CMAKE_CXX_FLAGS_RELEASE_INIT: ${CMAKE_CXX_FLAGS_RELEASE_INIT}")
elseif(${CMAKE_BUILD_TYPE} MATCHES Debug)
	message("CMAKE_CXX_FLAGS_DEBUG_INIT: ${CMAKE_CXX_FLAGS_DEBUG_INIT}")
endif()

if (WIN32 AND NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(FATAL_ERROR "Windows builds only support 32bit for now")
endif()

include_directories(.)

add_library(agssqlite SHARED
        agssqliteplugin.cpp
        AgsSQLiteDB.cpp
        AgsSQLiteDB.h
        sqlite3.c
        sqlite3.h
        )


target_compile_definitions(agssqlite PRIVATE AGSSQLITE_EXPORTS)
target_include_directories(agssqlite PRIVATE ../../ags/Engine/)
target_include_directories(agssqlite PRIVATE ../../ags/Common/)

install(TARGETS agssqlite RUNTIME DESTINATION lib)