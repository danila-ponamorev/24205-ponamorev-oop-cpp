#cmake_minimum_required(VERSION 3.12)
#project(task-0-b)

cmake_minimum_required(VERSION 3.14)
project(test-task-1-a)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test-task-1-a tests.cpp bit_array.cpp)

target_link_libraries(test-task-1-a GTest::gtest_main)


add_test(NAME MainTest COMMAND test-task-1-a)
