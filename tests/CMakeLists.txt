# Add test executable
add_executable(unit_tests
  unit/main.cpp
  unit/core/GameTest.cpp
)

# Link test libraries
target_link_libraries(unit_tests
  PRIVATE
  gtest
  gtest_main
  ${PROJECT_NAME}_lib
)

include(GoogleTest)
gtest_discover_tests(unit_tests)