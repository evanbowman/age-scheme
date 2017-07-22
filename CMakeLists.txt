cmake_minimum_required(VERSION 2.8)

project(scheme-sge)

set(CMAKE_BUILD_TYPE Release)

add_executable(${PROJECT_NAME} "scheme-sge.c")

target_link_libraries(${PROJECT_NAME} "-lsge -lchibi-scheme")

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
