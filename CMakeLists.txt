cmake_minimum_required(VERSION 3.28)
project(ZBilliards LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(SFML_BUILD_SHARED_LIBS ON)
option(SFML_STATIC_LIBRARIES OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

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
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
	zsprite resource_manager timed_event_manager jwzsfml jwzhpp
	sfml-audio sfml-graphics sfml-window sfml-system
)


add_custom_target(CopyResources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
)
add_dependencies(${PROJECT_NAME} CopyResources)


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})


if(WIN32)
	if(CMAKE_GENERATOR_PLATFORM MATCHES "x64")
		set(SFML_ARCH x64)
	elseif(CMAKE_GENERATOR_PLATFORM MATCHES "Win32")
		set(SFML_ARCH x86)
	elseif(CMAKE_GENERATOR_PLATFORM MATCHES "ARM64")
		set(SFML_ARCH arm64)
	else()
		message(FATAL_ERROR "Unsupported Windows platform: ${CMAKE_GENERATOR_PLATFORM}")
	endif()

	add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_SOURCE_DIR}/extlibs/bin/${SFML_ARCH}/openal32.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>

		COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_BINARY_DIR}/lib/sfml*.dll
            $<TARGET_FILE_DIR:${PROJECT_NAME}>

        VERBATIM
    )

elseif(APPLE)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_BINARY_DIR}/lib/libsfml-*.2.6.2.dylib
            $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_SOURCE_DIR}/extlibs/libs-osx/Frameworks/OpenAL.framework/Versions/A/OpenAL
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/

        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_SOURCE_DIR}/extlibs/libs-osx/Frameworks/FLAC.framework/Versions/A/FLAC
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/

        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_SOURCE_DIR}/extlibs/libs-osx/Frameworks/freetype.framework/Versions/A/freetype
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/

        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_SOURCE_DIR}/extlibs/libs-osx/Frameworks/ogg.framework/Versions/A/ogg
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/

        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_SOURCE_DIR}/extlibs/libs-osx/Frameworks/vorbis*.framework/Versions/A/vorbis*
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/

        VERBATIM
    )

elseif(UNIX AND NOT APPLE)
    # Linux: copy SFML .so files into the executable folder
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        
        COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_BINARY_DIR}/lib/libsfml*.so.*
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
          
		COMMAND ${CMAKE_COMMAND} -E copy
			"${CMAKE_SOURCE_DIR}/Linux README.txt"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>
        
        VERBATIM
    )
endif()
