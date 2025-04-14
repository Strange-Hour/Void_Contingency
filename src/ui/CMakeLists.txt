# UI module CMake configuration
set(UI_SOURCES

  # Add UI source files here
)

set(UI_HEADERS

  # Add UI header files here
)

# Create UI library
add_library(ui STATIC ${UI_SOURCES} ${UI_HEADERS})

# Include directories
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set C++ standard
set_target_properties(ui PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)