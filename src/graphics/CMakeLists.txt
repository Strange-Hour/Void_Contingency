# Graphics module CMake configuration
set(GRAPHICS_SOURCES
  Renderer.cpp
  Sprite.cpp
  Color.cpp
)

set(GRAPHICS_HEADERS
  Renderer.hpp
  Sprite.hpp
  Color.hpp
)

# Create graphics library
add_library(graphics STATIC ${GRAPHICS_SOURCES} ${GRAPHICS_HEADERS})

# Link SDL2
target_link_libraries(graphics PRIVATE SDL2::SDL2-static)

# Add SDL2_image include directories - try all possible locations
target_include_directories(graphics PRIVATE
  ${SDL2_image_SOURCE_DIR} # The root directory
  ${SDL2_image_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/_deps/sdl2_image-build # For generated headers
  ${CMAKE_BINARY_DIR}/_deps/sdl2_image-src # And this path
  ${CMAKE_BINARY_DIR}/_deps/sdl2_image-src/include # Try this path even if it doesn't exist
  ${CMAKE_BINARY_DIR}/include # Generated include directory
  ${CMAKE_BINARY_DIR}/include/SDL2 # Generated SDL2 include directory
)

# Add compile definition to use the correct SDL_image.h path
target_compile_definitions(graphics PRIVATE
  SDL_IMAGE_USE_COMMON_BACKEND
)

# Create a custom imported target for SDL2_image-static
if(NOT TARGET SDL2_image_static_imported)
  add_library(SDL2_image_static_imported STATIC IMPORTED)

  # Use the correct path to the SDL2_image library
  set_target_properties(SDL2_image_static_imported PROPERTIES
    IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/lib/Debug/SDL2_imaged.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_image_SOURCE_DIR};${SDL2_image_BINARY_DIR};${CMAKE_BINARY_DIR}/_deps/sdl2_image-build;${CMAKE_BINARY_DIR}/_deps/sdl2_image-src"
  )
endif()

# Link against the imported target
target_link_libraries(graphics PRIVATE SDL2_image_static_imported)

# Add SDL2 include directories
target_include_directories(graphics PRIVATE
  ${SDL2_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/_deps/sdl2-src/include # Direct path to SDL2 include
)

# Include directories
target_include_directories(graphics PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include # Add the main include directory
)

# Set C++ standard
set_target_properties(graphics PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

# Comment out the SDL2_SHARED property setting as it's causing conflicts
# set_property(TARGET graphics PROPERTY INTERFACE_SDL2_SHARED OFF)