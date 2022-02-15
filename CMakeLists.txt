project(VOLLEY)

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wextra -Wall -pedantic --std=c++17 -g2")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wextra -Wall -pedantic --std=c++17 -O3")

include_directories(include)

set(SRC_FILES src/main.cpp
  src/volley.cpp
  src/entity.cpp
  src/utils.cpp
  src/ball.cpp
  src/character.cpp
  src/score.cpp
  src/sound.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


link_libraries(sfml-graphics sfml-window sfml-system sfml-audio pthread X11)


add_executable(volley ${SRC_FILES})


add_custom_command(TARGET volley POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/resources/"
      $<TARGET_FILE_DIR:volley>)
