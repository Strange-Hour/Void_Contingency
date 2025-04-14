# This file patches SDL2_image's CMake configuration to prevent property conflicts

# Function to patch SDL2_image's CMakeLists.txt
function(patch_sdl2_image)
  message(STATUS "Patching SDL2_image CMake configuration...")

  # Get the source directory of SDL2_image
  FetchContent_GetProperties(SDL2_image SOURCE_DIR SDL2_image_SOURCE_DIR)

  if(NOT SDL2_image_SOURCE_DIR)
    message(WARNING "SDL2_image source directory not found, cannot apply patch")
    return()
  endif()

  message(STATUS "SDL2_image source directory: ${SDL2_image_SOURCE_DIR}")

  # Check if the CMakeLists.txt file exists
  if(NOT EXISTS "${SDL2_image_SOURCE_DIR}/CMakeLists.txt")
    message(WARNING "SDL2_image CMakeLists.txt not found at ${SDL2_image_SOURCE_DIR}/CMakeLists.txt")
    return()
  endif()

  # Read the original CMakeLists.txt
  file(READ "${SDL2_image_SOURCE_DIR}/CMakeLists.txt" SDL2_IMAGE_CMAKE_CONTENT)

  # Replace any code that sets the SDL2_SHARED property
  string(REPLACE "set_target_properties(SDL2_image PROPERTIES"
    "# Patched to prevent SDL2_SHARED property conflicts\nset_target_properties(SDL2_image PROPERTIES"
    SDL2_IMAGE_CMAKE_CONTENT "${SDL2_IMAGE_CMAKE_CONTENT}")

  # Replace any code that sets INTERFACE_SDL2_SHARED
  string(REPLACE "INTERFACE_SDL2_SHARED TRUE"
    "INTERFACE_SDL2_SHARED FALSE"
    SDL2_IMAGE_CMAKE_CONTENT "${SDL2_IMAGE_CMAKE_CONTENT}")

  # Add a block at the end of the file to force INTERFACE_SDL2_SHARED to OFF
  string(APPEND SDL2_IMAGE_CMAKE_CONTENT "\n\n# Force INTERFACE_SDL2_SHARED to OFF for all targets\nforeach(target SDL2_image SDL2_image-static)\n  if(TARGET \${target})\n    set_property(TARGET \${target} PROPERTY INTERFACE_SDL2_SHARED OFF)\n  endif()\nendforeach()\n")

  # Write the modified CMakeLists.txt back
  file(WRITE "${SDL2_image_SOURCE_DIR}/CMakeLists.txt" "${SDL2_IMAGE_CMAKE_CONTENT}")

  message(STATUS "SDL2_image CMake configuration patched successfully")
endfunction()

# Function to create SDL2_image target after configuration
function(create_sdl2_image_target)
  message(STATUS "Creating SDL2_image target...")

  # Get SDL2_image properties
  FetchContent_GetProperties(SDL2_image SOURCE_DIR SDL2_image_SOURCE_DIR BINARY_DIR SDL2_image_BINARY_DIR)

  if(NOT SDL2_image_SOURCE_DIR OR NOT SDL2_image_BINARY_DIR)
    message(WARNING "SDL2_image source or binary directory not found, cannot create target")
    return()
  endif()

  message(STATUS "SDL2_image source directory: ${SDL2_image_SOURCE_DIR}")
  message(STATUS "SDL2_image binary directory: ${SDL2_image_BINARY_DIR}")

  # Check if the SDL2_image target already exists
  if(TARGET SDL2_image)
    message(STATUS "SDL2_image target already exists, updating properties")

    # Update the target properties
    set_target_properties(SDL2_image PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${SDL2_image_SOURCE_DIR};${SDL2_image_BINARY_DIR}"
    )
  else()
    # Create a custom interface library for SDL2_image
    add_library(SDL2_image INTERFACE)
    target_include_directories(SDL2_image INTERFACE
      ${SDL2_image_SOURCE_DIR}
      ${SDL2_image_BINARY_DIR}
    )
  endif()

  # Link against SDL2::SDL2-static
  target_link_libraries(SDL2_image INTERFACE SDL2::SDL2-static)

  # Explicitly set the INTERFACE_SDL2_SHARED property to OFF
  set_property(TARGET SDL2_image PROPERTY INTERFACE_SDL2_SHARED OFF)

  # Create the SDL2_image::SDL2_image alias if it doesn't exist
  if(NOT TARGET SDL2_image::SDL2_image)
    add_library(SDL2_image::SDL2_image ALIAS SDL2_image)
  endif()

  message(STATUS "SDL2_image target created successfully")
endfunction()