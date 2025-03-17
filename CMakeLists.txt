cmake_minimum_required(VERSION 3.20.0)

project(calculator)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(calculator ${SRC_FILES})

target_include_directories(calculator PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(calculator PRIVATE mpfr)
