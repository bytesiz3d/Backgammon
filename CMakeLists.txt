cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

add_compile_options(-w)

include(FetchContent)

option(BKG_USE_GLFW "Use the GLFW Implementation of the GUI" OFF)
option(BKG_USE_SFML "Use the SFML Implementation of the GUI" ON)

# Choosing the Implemntation:
if(BKG_USE_GLFW AND(BKG_USE_SFML))
	message(FATAL_ERROR "Can't use both implementations")
endif()

if(BKG_USE_GLFW AND NOT BKG_USE_SFML)
	list(APPEND BKG_GUI_FILES src/GUI/GUI_GLFW.cpp src/GUI/GUI_GLFW.h)
	add_compile_definitions(_GUI_GLFW_)
endif()

if(BKG_USE_SFML AND NOT BKG_USE_GLFW)
	list(APPEND BKG_GUI_FILES src/GUI/GUI_SFML.cpp src/GUI/GUI_SFML.h)
	add_compile_definitions(_GUI_SFML_)
endif()

# Project and Source Groups:
project(Backgammon)
add_executable(Backgammon
	src/main.cpp
	src/Utility/Utility.cpp
	src/Utility/Utility.h
	${BKG_GUI_FILES}
	src/Game/Game.cpp
	src/Game/Game.h
)

source_group("Utility"
	FILES
	src/Utility/Utility.cpp
	src/Utility/Utility.h
)

source_group("GUI"
	FILES
	${BKG_GUI_FILES}
)

source_group("Game"
	FILES
	src/Game/Game.cpp
	src/Game/Game.h
)

# OpenGL:
find_package(OpenGL REQUIRED)
target_link_libraries(Backgammon OpenGL::GL)

# GLFW:
if(BKG_USE_GLFW)
	FetchContent_Declare(GLFW
		GIT_REPOSITORY https://github.com/glfw/glfw
		GIT_TAG 3.3
	)
	FetchContent_MakeAvailable(GLFW)

	option(GLFW_BUILD_EXAMPLES OFF)
	option(GLFW_BUILD_TESTS OFF)
	option(GLFW_BUILD_DOCS OFF)
	option(GLFW_INSTALL OFF)
	option(GLFW_VULKAN_STATIC OFF)

	target_link_libraries(Backgammon glfw)
endif()

set(FETCHCONTENT_QUIET FALSE)
# SFML:
if(BKG_USE_SFML)
	FetchContent_Declare(SFML
		GIT_REPOSITORY https://github.com/SFML/SFML
		GIT_TAG 2.5.1
	)
	set(SFML_BUILD_AUDIO FALSE)
	set(SFML_BUILD_NETWORK FALSE)
	FetchContent_MakeAvailable(SFML)
	target_link_libraries(Backgammon sfml-main sfml-window sfml-graphics)

	FetchContent_Declare(imgui
		GIT_REPOSITORY https://github.com/ocornut/imgui
		GIT_TAG v1.80
	)
	FetchContent_MakeAvailable(imgui)

	FetchContent_Declare(imgui-sfml
		GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml
		GIT_TAG v2.5
	)
	set(IMGUI_DIR ${imgui_SOURCE_DIR})
	set(IMGUI_SFML_FIND_SFML OFF)
	set(IMGUI_SFML_IMGUI_DEMO ON)
	FetchContent_MakeAvailable(imgui-sfml)
	target_link_libraries(Backgammon ImGui-SFML::ImGui-SFML)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Backgammon)

target_compile_definitions(Backgammon PRIVATE UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS ASSET_DIR="${CMAKE_SOURCE_DIR}/assets")

if(MSVC AND BKG_USE_SFML)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
