cmake_minimum_required(VERSION 3.20.x)

project(ExtendedNative CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib")

include(cmake/minhook.cmake)

file(GLOB_RECURSE SRC_FILES
	"${SRC_DIR}/*.cpp"
	"${SRC_DIR}/*.hpp"
	"${SRC_DIR}/*.h"
)

add_library(${PROJECT_NAME} MODULE ${SRC_FILES})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

target_precompile_headers(${PROJECT_NAME} PUBLIC "${SRC_DIR}/pch.hpp")

target_compile_definitions(${PROJECT_NAME} PRIVATE "DEV_BUILD")

target_include_directories(${PROJECT_NAME} PRIVATE
	"${SRC_DIR}"
	"${minhook_SOURCE_DIR}/src/hde"
)

# set_target_properties(${PROJECT_NAME} PROPERTIES
# 	OUTPUT_NAME "ExtendedNative"
# 	SUFFIX ".red"
# )

target_link_libraries(${PROJECT_NAME} PRIVATE
	psapi.lib
	minhook
)

set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE "_CRT_SECURE_NO_WARNINGS"
	PRIVATE "NOMINMAX"
	PRIVATE "WIN32_LEAN_AND_MEAN"
	PRIVATE "/NODEFAULTLIB:LIBCMT"
)

file(GLOB_RECURSE ALL_SOURCE_FILES
	"${SRC_DIR}/*.cpp"
	"${SRC_DIR}/*.hpp"
	"${SRC_DIR}/*.h"
)

find_program(CLANG_FORMAT clang-format)

if(CLANG_FORMAT)
	add_custom_target(format
		COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
		COMMENT "Formatting code with clang-format"
	)

	add_custom_target(format-check
		COMMAND ${CLANG_FORMAT} --dry-run -Werror ${ALL_SOURCE_FILES}
		COMMENT "Checking code format with clang-format"
	)
else()
	message(WARNING "clang-format not found. Install it to use format target")
endif()