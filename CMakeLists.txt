cmake_minimum_required(VERSION 3.22)
project(DoomIIRPG)

set(CMAKE_CXX_STANDARD 20)

if (NOT ANDROID)
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
else()
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
endif()

add_compile_options(
    -fsigned-char         # Force char to be signed
)

if(MSVC)
	add_definitions("/D_CRT_SECURE_NO_WARNINGS")
endif()

if(UNIX)
	add_definitions("-Wno-write-strings")
endif()

if (NOT ANDROID)
	find_package(SDL2 REQUIRED)
	find_package(ZLIB REQUIRED)
	find_package(OpenAL REQUIRED)
	find_package(OpenGL REQUIRED)
endif ()

# Global identifiers for each project/target
set(HASH_LIBRARY_TGT_NAME           doomrpg2-Hash-Library)
add_subdirectory("${PROJECT_SOURCE_DIR}/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/third_party_libs/hash-library")