cmake_minimum_required(VERSION 3.12)
project(LemmaGen VERSION 1.3.0
                 DESCRIPTION "LemmaGen lemmatizer"
                 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)

# ========================
# SHARED LIBRARY
# ========================
file(GLOB lemmagen_src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

set(LINUX_LINKER_FLAGS "-Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro -Wl,--gc-sections")
set(MACOS_LINKER_FLAGS "-Wl,-dead_strip")

if (UNIX)
  set(WARNINGS -Wall -Weverything -Wno-c++98-compat -Wno-old-style-cast -Wno-c++98-compat-pedantic -Wno-cast-align -Wno-padded)
endif(UNIX)

add_library(lemmagen SHARED ${lemmagen_src_files})
target_compile_options(lemmagen PUBLIC ${WARNINGS} -ffunction-sections -fdata-sections -fstack-protector -fvisibility-inlines-hidden -static)

target_compile_definitions(lemmagen PRIVATE _FORTIFY_SOURCE=2)
target_include_directories(lemmagen INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(lemmagen PROPERTIES
    CXX_VISIBILITY_PRESET hidden)

# Set OS specific compile flags
if (APPLE)
    set_target_properties(lemmagen PROPERTIES LINK_FLAGS ${MACOS_LINKER_FLAGS})
endif(APPLE)

# ========================
# BINARY
# ========================

#add_executable(lemmagen_bin src/main.c)
#target_link_libraries(lemmagen_bin lemmagen)

# ========================
# TESTS
# ========================
include(FetchContent)
include(GoogleTest)

if (WIN32)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif(WIN32)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.1
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

enable_testing()
file(GLOB lemmagen_test_files ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)

add_executable(lemmagen_tests ${lemmagen_test_files})
target_link_libraries(lemmagen_tests lemmagen gtest gtest_main)
gtest_add_tests(TARGET lemmagen_tests AUTO)
