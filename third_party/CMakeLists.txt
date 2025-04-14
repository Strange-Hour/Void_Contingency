include(FetchContent)
include(${CMAKE_CURRENT_SOURCE_DIR}/sdl2_image_patch.cmake)

# Google Test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# SDL2
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.30.2
  OVERRIDE_FIND_PACKAGE
)

# Set SDL2 build options before making it available
set(SDL_STATIC ON CACHE BOOL "Build SDL2 as a static library" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Build SDL2 as a shared library" FORCE)
set(SDL_TEST OFF CACHE BOOL "Disable SDL2 tests" FORCE)
set(SDL2_DISABLE_SDL2MAIN OFF CACHE BOOL "Enable SDL2main" FORCE)

# Make SDL2 available
FetchContent_MakeAvailable(SDL2)

# Configure SDL2 for SDL2_image before declaring it
# Don't set SDL2_SHARED directly as it causes property conflicts
# Instead, we'll handle this through target properties
set(SDL2_DIR ${SDL2_BINARY_DIR} CACHE PATH "Path to SDL2 build directory" FORCE)
set(SDL2_INCLUDE_DIR ${SDL2_SOURCE_DIR}/include CACHE PATH "Path to SDL2 include directory" FORCE)
set(SDL2_LIBRARY SDL2::SDL2-static CACHE STRING "SDL2 library target" FORCE)

# Make SDL2 available to SDL2_image
set(SDL2_FOUND TRUE CACHE BOOL "SDL2 found" FORCE)
set(SDL2_INCLUDE_DIRS ${SDL2_SOURCE_DIR}/include CACHE STRING "SDL2 include directories" FORCE)
set(SDL2_LIBRARIES SDL2::SDL2-static CACHE STRING "SDL2 libraries" FORCE)

# SDL2_image
FetchContent_Declare(
  SDL2_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG release-2.6.3
  OVERRIDE_FIND_PACKAGE
)

# Set SDL2_image build options before making it available
set(SDL2IMAGE_STATIC ON CACHE BOOL "Build SDL2_image as a static library" FORCE)
set(SDL2IMAGE_SHARED OFF CACHE BOOL "Build SDL2_image as a shared library" FORCE)
set(SDL2IMAGE_VENDORED ON CACHE BOOL "Use vendored libraries" FORCE)
set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "Disable SDL2_image samples" FORCE)
set(SDL2IMAGE_TESTS OFF CACHE BOOL "Disable SDL2_image tests" FORCE)

# Add a custom definition to prevent SDL2_image from setting INTERFACE_SDL2_SHARED
add_compile_definitions(SDL2IMAGE_DISABLE_SHARED_PROPS)

# Explicitly set SDL2_SHARED to OFF for the entire project
set(SDL2_SHARED OFF CACHE BOOL "Build SDL2 as a shared library" FORCE)

# Get SDL2_image properties
FetchContent_GetProperties(SDL2_image)

# We need to modify the FetchContent behavior for SDL2_image
# to allow patching before configuration
if(NOT SDL2_image_POPULATED)
  message(STATUS "SDL2_image not yet populated")

  # Get the content properties first
  FetchContent_GetProperties(SDL2_image)

  # For CMake 4.0 compatibility, we need to use a different approach
  # Instead of manually setting FETCHCONTENT_SOURCE_DIR_SDL2_IMAGE and then calling FetchContent_Populate,
  # we'll use FetchContent_MakeAvailable and then apply our patch

  # First, unset any manual source directory that might be causing issues
  unset(FETCHCONTENT_SOURCE_DIR_SDL2_IMAGE CACHE)

  # Use FetchContent_MakeAvailable to download and configure SDL2_image
  message(STATUS "Using FetchContent_MakeAvailable for SDL2_image...")
  FetchContent_MakeAvailable(SDL2_image)

  # Now apply our patch to SDL2_image's CMake configuration
  message(STATUS "Applying patch to SDL2_image...")
  patch_sdl2_image()

  # No need to add_subdirectory since FetchContent_MakeAvailable already did that

  # Make SDL2_image variables available to the rest of the project
  set(SDL2_image_SOURCE_DIR ${SDL2_image_SOURCE_DIR} PARENT_SCOPE)
  set(SDL2_image_BINARY_DIR ${SDL2_image_BINARY_DIR} PARENT_SCOPE)
else()
  # If already populated, just make the variables available
  set(SDL2_image_SOURCE_DIR ${SDL2_image_SOURCE_DIR} PARENT_SCOPE)
  set(SDL2_image_BINARY_DIR ${SDL2_image_BINARY_DIR} PARENT_SCOPE)
endif()

# Make SDL2 and SDL2_image available to the rest of the project
if(SDL2_FOUND)
  # Export SDL2 targets
  install(TARGETS SDL2-static SDL2main
    EXPORT SDL2Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
  )
endif()

if(SDL2_image_FOUND)
  # We're not using our custom SDL2_image target anymore
  # Instead, we're using direct linking in the graphics library
  message(STATUS "SDL2_image found and will be used directly by the graphics library")

  # Make SDL2_image variables available to the rest of the project
  set(SDL2_image_SOURCE_DIR ${SDL2_image_SOURCE_DIR} PARENT_SCOPE)
  set(SDL2_image_BINARY_DIR ${SDL2_image_BINARY_DIR} PARENT_SCOPE)
endif()