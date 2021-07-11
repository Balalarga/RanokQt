#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QSplitter>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>

#include "Gui/ToggleButton.h"

#include "Gui/Opengl/SceneView.h"
#include "Gui/CodeEditor/CodeEditor.h"
#include "Gui/CodeEditor/LineEditor.h"

#include "Language/Parser.h"

#include "TaskThread.h"
#include "Gui/LinearGradientModel.h"


enum class ObjectType
{
    Cube, Square
};

enum class Mode
{
    Common, Line
};


class AppWindow : public QWidget
{
    Q_OBJECT

public:
    AppWindow(QWidget *parent = nullptr);
    ~AppWindow();


protected:
    bool eventFilter(QObject *obj, QEvent *event) override;


private slots:
    void OpenFile();
    void Compute();
    void SwitchEditorMode();
    void SwitchModelMode();
    void ImageChanged(QString name);
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

    QLabel* _editorMode1Label;
    QLabel* _editorMode2Label;
    ToggleButton* m_editorModeButton;

    QLabel* _modelLabel;
    QLabel* _imageLabel;
    ToggleButton* m_imageModeButton;

    QComboBox* _imageType;
    QStringListModel* _imageTypeModel;
    QSpinBox* _spaceDepth;


    QPushButton* m_addLineButton;
};
#endif // WIDGET_H
