cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE MinSizeRel)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-w)

option(BKG_USE_GLFW "Use the GLFW Implementation of the GUI" ON)
option(BKG_USE_SFML "Use the SFML Implementation of the GUI" OFF)

if(BKG_USE_GLFW AND (BKG_USE_SFML))
  message(FATAL_ERROR "Can't use both implementations")
endif()

if(BKG_USE_GLFW AND (NOT (BKG_USE_SFML)))
  list (APPEND BKG_GUI_FILES GUI/GUI_GLFW.cpp GUI/GUI_GLFW.h)
  add_compile_definitions(_GUI_GLFW_)
endif()

if(BKG_USE_SFML AND (NOT (BKG_USE_GLFW)))
  list (APPEND BKG_GUI_FILES GUI/GUI_SFML.cpp GUI/GUI_SFML.h)
  add_compile_definitions(_GUI_SFML_)
endif()

project(Backgammon)
add_executable(Backgammon
  source/main.cpp
  Utility/Utility.cpp
  Utility/Utility.h
  ${BKG_GUI_FILES}
  Game/Game.cpp
  Game/Game.h     
  )

source_group("Utility"
  FILES
  Utility/Utility.cpp
  Utility/Utility.h
  )

source_group("GUI"
  FILES
  ${BKG_GUI_FILES}
  )

source_group("Game"
  FILES
  Game/Game.cpp
  Game/Game.h
  )

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(Backgammon OpenGL::GL)

# GLFW
if(BKG_USE_GLFW)
  option(GLFW_BUILD_EXAMPLES OFF)
  option(GLFW_BUILD_TESTS    OFF)
  option(GLFW_BUILD_DOCS 	   OFF)
  option(GLFW_INSTALL	   OFF)
  option(GLFW_VULKAN_STATIC  OFF)

  add_subdirectory(deps/glfw)
  target_link_libraries(Backgammon glfw)
endif()

# SFML
if(BKG_USE_SFML)
  add_subdirectory(deps/SFML)
  target_link_libraries(Backgammon sfml-system sfml-main sfml-window sfml-graphics sfml-audio)

  set(SFML_BUILD_NETWORK FALSE CACHE BOOL "No need to build the Network module." FORCE)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Backgammon)

target_include_directories(Backgammon PRIVATE deps/SFML/include)

target_compile_definitions(Backgammon PRIVATE UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS)

  
