#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QSplitter>
#include <QPushButton>

#include "Opengl/sceneview.h"
#include "CodeEditor/codeeditor.h"
#include "CodeEditor/lineeditor.h"

#include "Language/Parser.h"

#include "taskthread.h"


enum class ObjectType
{
    Cube, Square
};

enum class Mode
{
    Common, Line
};


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


protected:
    bool eventFilter(QObject *obj, QEvent *event) override;


private slots:
    void OpenFile();
    void Compute();
    void SwitchMode();
    void ComputeLine(QString line);


private:
    Mode m_mode;
    QToolBar* m_toolBar;

    SceneView* m_sceneView;

    CodeEditor* m_codeEditor;
    LineEditor* m_lineEditor;
    ModelThread* m_modelThread;
    ImageThread* m_imageThread;

    Parser m_parser;
    Program* m_program;
    Parser m_lineParser;
    Program* m_lineProgram;
    Zone _currentZone;

    QPushButton* m_modeButton;
    QPushButton* m_addLineButton;
};
#endif // WIDGET_H
