# Core module CMake configuration
set(CORE_SOURCES

  # Add core source files here
)

set(CORE_HEADERS

  # Add core header files here
)

# Create core library
add_library(core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

# Include directories
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set C++ standard
set_target_properties(core PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)