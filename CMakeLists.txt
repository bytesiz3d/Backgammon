cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

add_compile_options(-w)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# Project and Source Groups:
project(Backgammon)
add_executable(Backgammon
	src/main.cpp
	src/Utility/Utility.cpp
	src/Utility/Utility.h
	src/GUI/Roboto-Medium-ttf.h
	src/GUI/GUI_SFML.h
	src/GUI/GUI_SFML.cpp
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

# SFML:
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

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Backgammon)

target_compile_definitions(Backgammon PRIVATE UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS ASSET_DIR="${CMAKE_SOURCE_DIR}/assets")

if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
