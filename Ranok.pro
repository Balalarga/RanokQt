TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

win32{
    LIBS += -lmingw32
    LIBS += -lopengl32 -lGLEW32 -lGLU32
    LIBS += -L"..\libs"
    INCLUDEPATH += "..\include"
}
unix{
    LIBS += -lGL -lGLEW -lGLU
}
LIBS += -lSDL2main -lSDL2 -lSDL2_image

SOURCES += \
        Lang/Example.cpp \
        Lang/langfunctions.cpp \
        Lang/lexer.cpp \
        Lang/parser.cpp \
        Lang/program.cpp \
        Render/cube.cpp \
        Render/window.cpp \
        main.cpp

HEADERS += \
    Lang/langfunctions.h \
    Lang/lexer.h \
    Lang/node.h \
    Lang/parser.h \
    Lang/program.h \
    Lang/token.h \
    Render/cube.h \
    Render/glInclude.h \
    Render/window.h

DISTFILES += \
    Examples/sphere.txt \
    Examples/sphere_values.txt \
    Examples/1.txt
