# Game module CMake configuration
set(GAME_SOURCES

  # Add game source files here
)

set(GAME_HEADERS

  # Add game header files here
)

# Create game library
add_library(game STATIC ${GAME_SOURCES} ${GAME_HEADERS})

# Include directories
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set C++ standard
set_target_properties(game PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)