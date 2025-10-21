#cmake_minimum_required(VERSION 3.12)
#project(task-0-b)

cmake_minimum_required(VERSION 3.14)
project(test-task-0-b)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test-task-0-b tests.cpp main_wrapper.cpp text_processor.cpp file_reader.cpp word_frequency_analyzer.cpp csv_writer.cpp)

add_executable(task-0-b main.cpp main_wrapper.cpp text_processor.cpp file_reader.cpp word_frequency_analyzer.cpp csv_writer.cpp)

target_link_libraries(test-task-0-b GTest::gtest_main)


add_test(NAME MainTest COMMAND test-task-0-b)

