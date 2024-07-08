cmake_minimum_required(VERSION 3.28)
project(2D_graph_viewer CXX)

find_package(Qt6 CONFIG REQUIRED)
set(THIRD_PARTY_LIBS ${THIRD_PARTY_LIBS} Qt6::Widgets Qt6::Core Qt6::Gui)

set(VIEWER_COMPILE_FLAGS /W4 /wd4250)
set(CMAKE_AUTOMOC TRUE)

add_executable(${PROJECT_NAME} WIN32 src/main.cpp "src/viewmodel/source/GraphConnector.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE ${THIRD_PARTY_LIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${VIEWER_COMPILE_FLAGS})

function(create_viewer_library target)
    add_library(${target} STATIC)
    
    target_link_libraries(${target} PRIVATE ${THIRD_PARTY_LIBS})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    file(GLOB_RECURSE sources CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/source/*
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*)
    target_sources(${target} PRIVATE ${sources})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${target})
    target_compile_options(${target} PRIVATE ${VIEWER_COMPILE_FLAGS})

endfunction(create_viewer_library)

add_subdirectory(src/view)
# add_subdirectory(src/common)
add_subdirectory(src/model)
add_subdirectory(src/viewmodel)

