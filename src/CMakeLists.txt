# Add source files
file(GLOB_RECURSE SOURCES
  "*.cpp"
  "core/*.cpp"
  "game/*.cpp"
  "graphics/*.cpp"
  "input/*.cpp"
  "network/*.cpp"
  "physics/*.cpp"
  "ui/*.cpp"
  "utils/*.cpp"
)

# Create library target
add_library(${PROJECT_NAME}_lib STATIC ${SOURCES})

# Include directories for library
target_include_directories(${PROJECT_NAME}_lib
  PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Create main executable
add_executable(${PROJECT_NAME} main.cpp)

# Link main executable with library
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  ${PROJECT_NAME}_lib
  SDL2::SDL2

  # Add external libraries here
)