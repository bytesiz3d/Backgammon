cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-w)

option(BKG_USE_GLFW "Use the GLFW Implementation of the GUI" OFF)
option(BKG_USE_SFML "Use the SFML Implementation of the GUI" ON)

# Choosing the Implemntation:
if(BKG_USE_GLFW AND (BKG_USE_SFML))
  message(FATAL_ERROR "Can't use both implementations")
endif()

if(BKG_USE_GLFW AND (NOT (BKG_USE_SFML)))
  list(APPEND BKG_GUI_FILES src/GUI/GUI_GLFW.cpp src/GUI/GUI_GLFW.h)
  add_compile_definitions(_GUI_GLFW_)
endif()

if(BKG_USE_SFML AND (NOT (BKG_USE_GLFW)))
  list (APPEND BKG_GUI_FILES src/GUI/GUI_SFML.cpp src/GUI/GUI_SFML.h)
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
  option(GLFW_BUILD_EXAMPLES OFF)
  option(GLFW_BUILD_TESTS    OFF)
  option(GLFW_BUILD_DOCS 	   OFF)
  option(GLFW_INSTALL	   OFF)
  option(GLFW_VULKAN_STATIC  OFF)

  add_subdirectory(deps/glfw)
  target_link_libraries(Backgammon glfw)
endif()

# SFML:
if(BKG_USE_SFML)
  if (NOT SFML_LIBRARIES)
    list(APPEND SFML_LIBRARIES sfml-main sfml-window sfml-graphics sfml-audio)
  endif()

  set(SFML_BUILD_NETWORK        FALSE CACHE BOOL "No need to build the Network module." FORCE)
  set(BUILD_SHARED_LIBS         FALSE CACHE BOOL "Static linking." FORCE)
  set(SFML_USE_STATIC_STD_LIBS   TRUE CACHE BOOL "Static linking." FORCE)
  set(SFML_USE_SYSTEM_DEPS       TRUE CACHE BOOL "Use system dependencies." FORCE)
  
  add_subdirectory(deps/SFML)
  target_include_directories(Backgammon PRIVATE deps/SFML/include)
  target_link_libraries(Backgammon ${SFML_LIBRARIES})
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Backgammon)

target_compile_definitions(Backgammon PRIVATE UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS)

if (MSVC AND BKG_USE_SFML)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
