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

# Create main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME}
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
  PRIVATE

  # Add external libraries here
)