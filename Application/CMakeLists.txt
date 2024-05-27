# Set minimum Version
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# Set Project Name
set(PROJECT_NAME Application)

# Set Project
project (
	${PROJECT_NAME} 
	VERSION 0.0.0 
	DESCRIPTION "MasterOfDnD - Game for online DnD games"
)

# External libraries
find_package(Stb REQUIRED)
find_package(Boost REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_path(GZIP_HPP_INCLUDE_DIRS "gzip/compress.hpp")
find_path(RAPIDXML_INCLUDE_DIRS "rapidxml/rapidxml.hpp")

# Include directories
include_directories(./inc)

# Source Files
set(SOURCE_FILES
	./src/main.cpp
	./src/app.cpp

	./src/render/render_core.cpp
	./src/render/gl/gl_buffer.cpp
	./src/render/gl/gl_camera.cpp
	./src/render/gl/gl_program.cpp
	./src/render/gl/gl_texture.cpp
	./src/render/gl/gl_framebuffer.cpp

	./src/file_system/fs_helper.cpp

	./src/ui/ui_core.cpp
	./src/ui/ui_viewport.cpp
	./src/ui/ui_nav_elements.cpp

	./src/engine/mesh.cpp
	./src/engine/logger.cpp
	./src/engine/base_texture.cpp
	./src/engine/mat_controller.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} "inc/engine/mat_controller.h")
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE implot::implot)
target_link_libraries(${PROJECT_NAME} PRIVATE imguizmo::imguizmo)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC  ${Boost_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${GZIP_HPP_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${RAPIDXML_INCLUDE_DIRS})

if(WIN32)
# set_target_properties(${PROJECT_NAME} PROPERTIES 
#     LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")
elseif(UNIX)

endif()

# Copy resources folder
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/resources
                ${CMAKE_CURRENT_BINARY_DIR}/resources)