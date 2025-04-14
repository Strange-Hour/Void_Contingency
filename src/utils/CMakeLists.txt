# Utils module CMake configuration
set(UTILS_SOURCES

  # Add utils source files here
)

set(UTILS_HEADERS

  # Add utils header files here
)

# Create utils library
add_library(utils STATIC ${UTILS_SOURCES} ${UTILS_HEADERS})

# Include directories
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set C++ standard
set_target_properties(utils PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)