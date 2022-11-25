cmake_minimum_required(VERSION 3.20)

include("cmake/sourcelist.cmake")

option(COPY_BUILD "Copy the build output to the Fallout 4 directory." OFF)

macro(set_from_environment VARIABLE)
	if (NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif ()
endmacro()

set_from_environment(Fallout4Path)

add_project(
	TARGET_TYPE LIBRARY
	LIBRARY_TYPE SHARED
  PROJECT rustyf4se
  VERSION 1.0.0
	INCLUDE_DIRECTORIES
		"${CMAKE_CURRENT_BINARY_DIR}/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/src/cpp"
	GROUPED_FILES
		${SOURCES}
	MISC_FILES
		"${CMAKE_CURRENT_BINARY_DIR}/include/Version.h"
		"${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	PRECOMPILED_HEADERS
		"src/cpp/PCH.h"
)

if(NOT TARGET CommonLibF4)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../CommonLibF4" CommonLibF4)
endif()

find_program(CXXBRIDGE cxxbridge REQUIRED PATHS $ENV{HOME}/.cargo/bin)
message(STATUS "Found cxxbridge: ${CXXBRIDGE}")
find_program(CARGO cargo REQUIRED PATHS $ENV{HOME}/.cargo/bin)
message(STATUS "Found cargo: ${CARGO}")
find_package(spdlog REQUIRED CONFIG)

# Generate CXX Bridge files.
set(CXXBRIDGE_IN ${CMAKE_CURRENT_SOURCE_DIR}/src/rs/bridge.rs)
set(CXXBRIDGE_SOURCE_OUT ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/_cxxbridge.cpp)
set(CXXBRIDGE_HEADER_OUT ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/_cxxbridge.h)
set(CXXBRIDGE_OUT ${CMAKE_CURRENT_SOURCE_DIR}/src/cpp/_cxx.h)
set(CXXBRIDGE_ARGS "--cxx-impl-annotations" "EXPORT_THIS")
add_custom_command(
  OUTPUT ${CXXBRIDGE_OUT} ${CXXBRIDGE_SOURCE_OUT} ${CXXBRIDGE_HEADER_OUT} BINDING_1
  COMMAND ${CXXBRIDGE} --header --output ${CXXBRIDGE_OUT}
  COMMAND ${CXXBRIDGE} ${CXXBRIDGE_IN} ${CXXBRIDGE_ARGS} --header --output ${CXXBRIDGE_HEADER_OUT}
  COMMAND ${CXXBRIDGE} ${CXXBRIDGE_IN} ${CXXBRIDGE_ARGS} --output ${CXXBRIDGE_SOURCE_OUT}
  COMMENT "Generating CXX Bridge: ${CXXBRIDGE_HEADER_OUT} ${CXXBRIDGE_SOURCE_OUT} ${CXXBRIDGE_OUT}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${CXXBRIDGE_IN})

# Generate Rust library.
set(RUST_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/target/release")
set(RUST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(RUST_LINKTIME_FILE "${CMAKE_CURRENT_SOURCE_DIR}/target/release/rust_static_library.lib")
add_custom_command(
  OUTPUT ${RUST_LINKTIME_FILE} RUST
  COMMAND ${CARGO} build --release
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating Rust library: ${RUST_LINKTIME_FILE}")

message(STATUS "Rust library: " ${RUST_LINKTIME_FILE})

# Force cmake to generate bindings
add_custom_target(
  ${PROJECT_NAME}_bindings ALL
  DEPENDS BINDING_1 BINDING_2
)
add_custom_target(
  ${PROJECT_NAME}_rust
  DEPENDS ${PROJECT_NAME}_bindings RUST
)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_rust)

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/include/Version.h"
	@ONLY
)

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/version.rc"
	@ONLY
)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC
    ${RUST_LINKTIME_FILE} 
  PRIVATE
    ws2_32
    userenv
    advapi32
    shell32
    bcrypt
    CommonLibF4::CommonLibF4
    spdlog::spdlog
  )

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES CXX_VISIBILITY_PRESET hidden
)

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE
    "/Zc:preprocessor"	# Enable preprocessor conformance mode
)

source_group(
	TREE "${CMAKE_CURRENT_BINARY_DIR}"
	FILES
		"${CMAKE_CURRENT_BINARY_DIR}/include/Version.h"
)

target_link_libraries(${PROJECT_NAME} PUBLIC msvcrt)

install(
  TARGETS
  ${PROJECT_NAME}
  ARCHIVE DESTINATION "lib"
  LIBRARY DESTINATION "lib"
  RUNTIME DESTINATION "bin")

install(
  DIRECTORY
  include/
  DESTINATION "include"
  FILES_MATCHING PATTERN "*.h")

if (COPY_BUILD)
	if (DEFINED Fallout4Path)
		add_custom_command(
			TARGET ${PROJECT_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${Fallout4Path}/F4SE/Plugins/
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${Fallout4Path}/F4SE/Plugins/
		)
	else ()
		message(
			WARNING
			"Variable Fallout4Path is not defined. Skipping post-build copy command."
		)
	endif ()
endif ()
