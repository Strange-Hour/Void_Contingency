# Physics module CMake configuration
set(PHYSICS_SOURCES

  # Add physics source files here
)

set(PHYSICS_HEADERS

  # Add physics header files here
)

# Create physics library
add_library(physics STATIC ${PHYSICS_SOURCES} ${PHYSICS_HEADERS})

# Include directories
target_include_directories(physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set C++ standard
set_target_properties(physics PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)