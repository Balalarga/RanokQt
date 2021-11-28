#ifndef RAY_MARCHING_SCENE_H
#define RAY_MARCHING_SCENE_H

#include <QSpinBox>
#include <fstream>

#include "Gui/ToggleButton.h"
#include "Gui/Opengl/RayMarchingView.h"
#include "Gui/CodeEditor/CodeEditor.h"

#include "Language/Parser.h"
#include "SpaceCalculatorThread.h"
#include "ClearableWidget.h"

class QProgressBar;

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
    void BuildMimage();
    void UpdateScreen();

    void RenderWidthChanged(int value);
    void RenderHeightChanged(int value);

    void BuildIteration(CalculatorMode mode, int batchStart, int count);


private:
    RayMarchingView* _sceneView;
    CodeEditor* _codeEditor;
    int _oldTabId;

    Parser _parser;
    Program* _program;
    OpenclCalculatorThread* _openclCalculator;
    ModelMetadata _metadata;

    QProgressBar* _progressBar;
    QSpinBox* _heightSpin;
    QSpinBox* _widthSpin;

    std::ofstream _resultFile;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // TESTSCREEN_H
