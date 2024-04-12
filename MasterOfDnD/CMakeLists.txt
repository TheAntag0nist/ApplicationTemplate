# Set minimum Version
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# Set Project Name
set(PROJECT_NAME MasterOfDnD)

# Set Project
project (
	${PROJECT_NAME} 
	VERSION 0.0.0 
	DESCRIPTION "MasterOfDnD - Game for online DnD games"
)

# External libraries
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Include directories
include_directories(./inc)

# Source Files
set(SOURCE_FILES
	./src/main.cpp
	./src/app.cpp

	./src/render/gl_buffer.cpp
	./src/render/gl_program.cpp
	./src/render/gl_texture.cpp

	./src/file_system/fs_helper.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES 
    LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")

# Copy resources folder
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/resources
                ${CMAKE_CURRENT_BINARY_DIR}/resources)