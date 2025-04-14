# Add subdirectories
add_subdirectory(graphics) # Only add graphics for now since it has content

# Add source files
file(GLOB_RECURSE SOURCES
  "*.cpp"
)

# Create library target
add_library(${PROJECT_NAME}_lib STATIC ${SOURCES})

# Include directories for library
target_include_directories(${PROJECT_NAME}_lib
  PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE
  ${SDL2_image_SOURCE_DIR} # The root directory
  ${SDL2_image_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/_deps/sdl2_image-build # For generated headers
  ${CMAKE_BINARY_DIR}/_deps/sdl2_image-src # And this path
  ${CMAKE_BINARY_DIR}/_deps/sdl2_image-src/include # Try this path even if it doesn't exist
  ${CMAKE_BINARY_DIR}/include # Generated include directory
  ${CMAKE_BINARY_DIR}/include/SDL2 # Generated SDL2 include directory
)

# Link library with dependencies
target_link_libraries(${PROJECT_NAME}_lib
  PUBLIC
  SDL2::SDL2-static
  graphics
)

# Instead of linking with SDL2_image::SDL2_image, use the imported target from graphics
# This avoids property conflicts
# target_link_libraries(${PROJECT_NAME}_lib
# PUBLIC
# SDL2_image::SDL2_image
# )

# Comment out the SDL2_SHARED property setting as it's causing conflicts
# set_property(TARGET ${PROJECT_NAME}_lib PROPERTY INTERFACE_SDL2_SHARED OFF)

# Create main executable
add_executable(${PROJECT_NAME} main.cpp)

# Link main executable with library
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  ${PROJECT_NAME}_lib
)