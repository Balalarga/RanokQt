#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QSplitter>
#include <QPushButton>

#include "ToggleButton.h"

#include "Opengl/SceneView.h"
#include "CodeEditor/CodeEditor.h"
#include "CodeEditor/LineEditor.h"

#include "Language/Parser.h"

#include "TaskThread.h"
#include "LinearGradientModel.h"


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
    LinearGradientModel* _linearGradModel;

    Zone _currentZone;
    MImageType _currentType;

    QLabel* _mode1Label;
    QLabel* _mode2Label;

    ToggleButton* m_modeButton;
    QPushButton* m_addLineButton;
};
#endif // WIDGET_H
