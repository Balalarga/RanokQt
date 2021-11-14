#ifndef TESTSCREEN_H
#define TESTSCREEN_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QSplitter>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QMap>
#include <QSlider>
#include <QProgressBar>
#include <QElapsedTimer>
#include <QQueue>
#include <QStringListModel>
#include <QCheckBox>

#include "Gui/ToggleButton.h"

#include "Gui/Opengl/TestView.h"
#include "Gui/CodeEditor/CodeEditor.h"

#include "Language/Parser.h"
#include "SpaceCalculatorThread.h"

#include "ClearableWidget.h"

class TestScreen : public ClearableWidget
{
    Q_OBJECT

public:
    TestScreen(QWidget *parent = nullptr);
    ~TestScreen();


public slots:
    void Cleanup() override;


private slots:
    void OpenFile();
    void Compute();


private:
    TestView* _sceneView;
    CodeEditor* _codeEditor;
    int _oldTabId;

    Parser _parser;
    Program* _program;

    QProgressBar* _progressBar;
};

#endif // TESTSCREEN_H
