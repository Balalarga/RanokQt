cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME Ranok)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include src)
include_directories(thirdParty/imgui thirdParty/imgui-sfml)

if(WIN32)
    link_directories(lib)
    set (PROJECT_LIBS mingw32 glew32 opengl32 glu32 Imm32)
else(WIN32)
    set (PROJECT_LIBS gl glew glu)
endif(WIN32)

set(IMGUI_SOURCES
    thirdParty/imgui/imgui.cpp
    thirdParty/imgui/imgui_widgets.cpp
    thirdParty/imgui/imgui_draw.cpp
    thirdParty/imgui/imgui_tables.cpp
    thirdParty/imgui-sfml/imgui-SFML.cpp)

set (PROJECT_LIBS ${PROJECT_LIBS} sfml-graphics sfml-window sfml-audio sfml-system sfml-network)
link_libraries(${PROJECT_LIBS})

file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.h examples/*.txt)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${IMGUI_SOURCES})

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/bin/ $<TARGET_FILE_DIR:${PROJECT_NAME}>)
