cmake_minimum_required(VERSION 3.18.0)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_ASM_COMPILER clang)
set(CMAKE_EXE_LINKER_FLAGS -fuse-ld=lld)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(facile-http LANGUAGES C CXX VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED
src/facile_http.cc
src/request.cc
src/response.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE
curl
)

# set version
set_target_properties(${PROJECT_NAME} PROPERTIES
VERSION ${PROJECT_VERSION}
SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)

install(TARGETS ${PROJECT_NAME} LIBRARY)
install(FILES
src/facile_http.h
src/request.h
src/response.h
DESTINATION include/facile_http
)

add_subdirectory(example)