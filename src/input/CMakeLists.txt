# Input module CMake configuration
set(INPUT_SOURCES

  # Add input source files here
)

set(INPUT_HEADERS

  # Add input header files here
)

# Create input library
add_library(input STATIC ${INPUT_SOURCES} ${INPUT_HEADERS})

# Include directories
target_include_directories(input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set C++ standard
set_target_properties(input PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)