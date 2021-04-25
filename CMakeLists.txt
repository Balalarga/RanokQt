cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME Ranok)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include src)


if(WIN32)
    link_directories(libs)
    set (PROJECT_LIBS mingw32 opengl32 glew32 glu32)
else(WIN32)
    set (PROJECT_LIBS gl glew glu)
endif(WIN32)

set (PROJECT_LIBS ${PROJECT_LIBS} SDL2main SDL2 SDL2_image)

link_libraries(${PROJECT_LIBS})

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.h data/*.txt)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/bin/ $<TARGET_FILE_DIR:${PROJECT_NAME}>)

#   if(EXISTS ${CMAKE_SOURCE_DIR}/data)
#       add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
#                         COMMAND ${CMAKE_COMMAND} -E copy_directory
#                             ${CMAKE_SOURCE_DIR}/data/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/data)
#   endif()
