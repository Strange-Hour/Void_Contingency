# Add test executable
add_executable(unit_tests
  unit/main.cpp

  # Add test files here
)

# Link test libraries
target_link_libraries(unit_tests
  PRIVATE
  gtest
  gtest_main
  ${PROJECT_NAME}_lib
)

# Add tests to CTest
add_test(NAME unit_tests COMMAND unit_tests)