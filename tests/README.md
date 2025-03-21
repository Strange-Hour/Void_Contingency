# Tests Directory

This directory contains all test-related code for the project:

- `unit/`: Unit tests for individual components and classes

  - Tests should be organized to mirror the main source structure
  - Each test file should correspond to a specific component or class
  - Focus on testing individual units in isolation

- `integration/`: Integration tests for testing multiple components together
  - Tests interactions between different systems
  - Verifies that components work together correctly
  - Tests larger features and workflows

Tests are built using Google Test framework and can be run through CMake's test runner (CTest).
