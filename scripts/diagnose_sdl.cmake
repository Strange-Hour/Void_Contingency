# Diagnostic script for SDL2 configuration issues
cmake_minimum_required(VERSION 3.20)

# This script should be run after configuring the project
# Usage: cmake -P scripts/diagnose_sdl.cmake
message(STATUS "Diagnosing SDL2 configuration issues...")

# Check if targets exist
if(NOT TARGET SDL2::SDL2)
  message(STATUS "SDL2::SDL2 target does not exist")
else()
  message(STATUS "SDL2::SDL2 target exists")
endif()

if(NOT TARGET SDL2::SDL2-static)
  message(STATUS "SDL2::SDL2-static target does not exist")
else()
  message(STATUS "SDL2::SDL2-static target exists")
endif()

if(NOT TARGET SDL2_image::SDL2_image)
  message(STATUS "SDL2_image::SDL2_image target does not exist")
else()
  message(STATUS "SDL2_image::SDL2_image target exists")
endif()

# Try to get properties if targets exist
if(TARGET SDL2::SDL2)
  get_target_property(SDL2_TYPE SDL2::SDL2 TYPE)
  message(STATUS "SDL2::SDL2 type: ${SDL2_TYPE}")

  get_target_property(SDL2_SHARED SDL2::SDL2 INTERFACE_SDL2_SHARED)

  if(SDL2_SHARED)
    message(STATUS "SDL2::SDL2 INTERFACE_SDL2_SHARED: ${SDL2_SHARED}")
  else()
    message(STATUS "SDL2::SDL2 INTERFACE_SDL2_SHARED not set or is FALSE")
  endif()
endif()

if(TARGET SDL2::SDL2-static)
  get_target_property(SDL2_STATIC_TYPE SDL2::SDL2-static TYPE)
  message(STATUS "SDL2::SDL2-static type: ${SDL2_STATIC_TYPE}")

  get_target_property(SDL2_STATIC_SHARED SDL2::SDL2-static INTERFACE_SDL2_SHARED)

  if(SDL2_STATIC_SHARED)
    message(STATUS "SDL2::SDL2-static INTERFACE_SDL2_SHARED: ${SDL2_STATIC_SHARED}")
  else()
    message(STATUS "SDL2::SDL2-static INTERFACE_SDL2_SHARED not set or is FALSE")
  endif()
endif()

if(TARGET SDL2_image::SDL2_image)
  get_target_property(SDL2_IMAGE_TYPE SDL2_image::SDL2_image TYPE)
  message(STATUS "SDL2_image::SDL2_image type: ${SDL2_IMAGE_TYPE}")

  get_target_property(SDL2_IMAGE_SHARED SDL2_image::SDL2_image INTERFACE_SDL2_SHARED)

  if(SDL2_IMAGE_SHARED)
    message(STATUS "SDL2_image::SDL2_image INTERFACE_SDL2_SHARED: ${SDL2_IMAGE_SHARED}")
  else()
    message(STATUS "SDL2_image::SDL2_image INTERFACE_SDL2_SHARED not set or is FALSE")
  endif()
endif()

if(TARGET VoidContingency_lib)
  get_target_property(VC_LIB_TYPE VoidContingency_lib TYPE)
  message(STATUS "VoidContingency_lib type: ${VC_LIB_TYPE}")

  get_target_property(VC_LIB_SHARED VoidContingency_lib INTERFACE_SDL2_SHARED)

  if(VC_LIB_SHARED)
    message(STATUS "VoidContingency_lib INTERFACE_SDL2_SHARED: ${VC_LIB_SHARED}")
  else()
    message(STATUS "VoidContingency_lib INTERFACE_SDL2_SHARED not set or is FALSE")
  endif()
endif()

message(STATUS "Diagnosis complete")