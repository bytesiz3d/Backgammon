cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-w)

project(Backgammon-gl)
add_executable(Backgammon-gl
  source/main.cpp
  Utility/Utility.cpp
  Utility/Utility.h
  GUI/GUI.cpp
  GUI/GUI.h
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
  GUI/GUI.cpp
  GUI/GUI.h
  )

source_group("Game"
  FILES
  Game/Game.cpp
  Game/Game.h
  )

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(Backgammon-gl OpenGL::GL)

# GLFW
add_subdirectory(deps/glfw)

# GLEW
# add_subdirectory(deps/glew)

target_link_libraries(Backgammon-gl glfw)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Backgammon-gl)

target_compile_definitions(Backgammon-gl PRIVATE UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS)

  
