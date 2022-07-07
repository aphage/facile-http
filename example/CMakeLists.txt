project(example LANGUAGES C CXX VERSION 1.0.0)

add_executable(${PROJECT_NAME}
main.cc
)

target_link_libraries(${PROJECT_NAME} PRIVATE
facile-http
)

target_include_directories(${PROJECT_NAME} PUBLIC
${CMAKE_SOURCE_DIR}/src
)