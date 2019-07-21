cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Release)

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

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(Backgammon-gl OpenGL::GL)

# GLFW
find_path(GLFW3_INCLUDE_PATH GLFW/glfw3.h
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/include)

find_library(GLFW3_LIBRARY
  NAMES glfw3 GLFW glfw3dll
  PATHS ${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/lib-vc2017
  )

include_directories(${GLFW3_INCLUDE_PATH})
target_link_libraries(Backgammon-gl
  ${GLFW3_LIBRARY})

# GLEW
find_path(GLEW_INCLUDE_DIRS GL/glew.h
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/glew/include)

find_library(GLEW_LIBRARIES
  NAMES GLEW glew32 glew glew32s
  PATHS ${CMAKE_CURRENT_SOURCE_DIR}/deps/glew/lib/Release/Win32
  )

include_directories(${GLEW_INCLUDE_DIRS})
target_link_libraries(Backgammon-gl
  ${GLEW_LIBRARIES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Backgammon-gl)

target_compile_definitions(Backgammon-gl PRIVATE UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS)

  
