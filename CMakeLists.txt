cmake_minimum_required(VERSION 3.28)
project(ZBilliards LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(SFML_BUILD_SHARED_LIBS ON)
option(SFML_STATIC_LIBRARIES OFF)
set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_INSTALL_RPATH "@executable_path;@executable_path/Frameworks;@executable_path/../Frameworks;@executable_path/../libs;@executable_path/libs")

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
set(SFML_BUILD_SHARED_LIBS ON CACHE BOOL "Build SFML as shared libraries")
set(SFML_STATIC_LIBRARIES OFF CACHE BOOL "Don't build static SFML libraries")
#set(SFML_USE_STATIC_STD_LIBS OFF CACHE BOOL "Link SFML against shared runtime")

#FetchContent_MakeAvailable(SFML)
FetchContent_Populate(SFML)
add_subdirectory(${SFML_SOURCE_DIR} ${SFML_BINARY_DIR})


FetchContent_Declare(sfml_shared_headers
    GIT_REPOSITORY https://github.com/johnnywz00/SFML-shared-headers.git
    GIT_TAG main
)
FetchContent_MakeAvailable(sfml_shared_headers)


file(GLOB SOURCES "src/*.cpp" "src/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES
	MACOSX_BUNDLE FALSE
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	INSTALL_RPATH "@executable_path;@executable_path/Frameworks;@executable_path/../Frameworks;;@executable_path/../libs;@executable_path/libs"
    BUILD_RPATH "@executable_path;@executable_path/Frameworks;@executable_path/../Frameworks;;@executable_path/../libs;@executable_path/libs"
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
	zsprite resource_manager timed_event_manager jwzsfml jwzhpp
	sfml-audio sfml-graphics sfml-window sfml-system
)


add_custom_target(CopyResources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
)
add_dependencies(${PROJECT_NAME} CopyResources)


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(WIN32)
	file(GLOB SFML_DYLIBS
		"${SFML_BINARY_DIR}/lib/Release/*"
	)
else()
	file(GLOB SFML_DYLIBS
		"${SFML_BINARY_DIR}/lib/*"
	)
endif()

message(STATUS "SFML libs: ${SFML_DYLIBS}")

message(STATUS "output dirs: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ||| ${RUNTIME_OUTPUT_DIRECTORY}")

message(STATUS "buildshared: ${BUILD_SHARED_LIBS}; sfmlbuildshared: ${SFML_BUILD_SHARED_LIBS}; sfmlstatic: ${SFML_STATIC_LIBRARIES}; staticstd: ${SFML_USE_STATIC_STD_LIBS}")

if(WIN32)
	if(NOT CMAKE_GENERATOR_PLATFORM)
        set(CMAKE_GENERATOR_PLATFORM "x64")
    endif()
    
	if(CMAKE_GENERATOR_PLATFORM MATCHES "x64")
		set(SFML_ARCH x64)
	elseif(CMAKE_GENERATOR_PLATFORM MATCHES "Win32")
		set(SFML_ARCH x86)
	elseif(CMAKE_GENERATOR_PLATFORM MATCHES "ARM64")
		set(SFML_ARCH arm64)
	else()
		message(FATAL_ERROR "Unsupported Windows platform: ${CMAKE_GENERATOR_PLATFORM}")
	endif()
	
	message(STATUS "arch: ${SFML_ARCH}")

	add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_directory
			${sfml_shared_headers_SOURCE_DIR}/sfml-dlls/${SFML_ARCH}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>

        VERBATIM
    )

elseif(APPLE)

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_directory
			${SFML_BINARY_DIR}/lib
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/libs/

        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SFML_SOURCE_DIR}/extlibs/libs-osx/Frameworks
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/Frameworks/

        VERBATIM
    )

elseif(UNIX AND NOT APPLE)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        
        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_BINARY_DIR}/lib/libsfml-audio.so.2.6
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
          
        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_BINARY_DIR}/lib/libsfml-graphics.so.2.6
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
          
        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_BINARY_DIR}/lib/libsfml-window.so.2.6
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
          
        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_BINARY_DIR}/lib/libsfml-system.so.2.6
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
          
		COMMAND ${CMAKE_COMMAND} -E copy
			"${CMAKE_SOURCE_DIR}/Linux README.txt"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
        
        VERBATIM
    )
endif()
