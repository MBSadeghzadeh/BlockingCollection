project(ProducerConsumerTest)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  ProducerConsumerTest

  simple_test.cpp
  random_test.cpp
  multi_consumer.cpp
  string_test.cpp
)
target_link_libraries(
ProducerConsumerTest  GTest::gtest_main
)

target_include_directories(ProducerConsumerTest PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(ProducerConsumerTest)