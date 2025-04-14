# Network module CMake configuration
set(NETWORK_SOURCES

  # Add network source files here
)

set(NETWORK_HEADERS

  # Add network header files here
)

# Create network library
add_library(network STATIC ${NETWORK_SOURCES} ${NETWORK_HEADERS})

# Include directories
target_include_directories(network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set C++ standard
set_target_properties(network PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)