cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME Ranok)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS OpenGL REQUIRED)

include_directories(src)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.h examples/*.txt *.qrc)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

add_subdirectory(./Core/RanokCoreLib RanokCore)


target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets
    Qt5::OpenGL RanokCore)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        opengl32 glu32)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        GL GLU)
endif()

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/)
