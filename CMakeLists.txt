cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME RanokQt)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.2\\mingw81_64\\lib\\cmake\\")

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(./Core/RanokCoreLib RanokCore)

include_directories(src)

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.h examples/*.txt *.qrc)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::OpenGL RanokCore)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        opengl32 glu32)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        GL GLU)
endif()

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/assets/
                   $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                  COMMAND ${CMAKE_COMMAND} -E copy_directory
                  ${CMAKE_SOURCE_DIR}/Core/Examples
                  $<TARGET_FILE_DIR:${PROJECT_NAME}>/Examples)
