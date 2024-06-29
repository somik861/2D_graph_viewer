cmake_minimum_required(VERSION 3.28)
project(2D_graph_viewer CXX)

find_package(Qt CONFIG REQUIRED)
set(LIBS ${LIBS} Qt::widgets)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBS})