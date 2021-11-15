#ifndef RAY_MARCHING_SCENE_H
#define RAY_MARCHING_SCENE_H

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

#include "Gui/Opengl/RayMarchingView.h"
#include "Gui/CodeEditor/CodeEditor.h"

#include "Language/Parser.h"
#include "SpaceCalculatorThread.h"

#include "ClearableWidget.h"

class RayMarchingScreen : public ClearableWidget
{
    Q_OBJECT

public:
    RayMarchingScreen(QWidget *parent = nullptr);
    ~RayMarchingScreen();


public slots:
    void Cleanup() override;


private slots:
    void OpenFile();
    void Compute();


private:
    RayMarchingView* _sceneView;
    CodeEditor* _codeEditor;
    int _oldTabId;

    Parser _parser;
    Program* _program;

    QProgressBar* _progressBar;
};

#endif // TESTSCREEN_H
