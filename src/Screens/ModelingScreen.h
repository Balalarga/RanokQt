#ifndef MODELINGSCREEN_H
#define MODELINGSCREEN_H

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

#include "Gui/Opengl/SceneView.h"
#include "Gui/CodeEditor/CodeEditor.h"

#include "Language/Parser.h"
#include "QSpaceCalculatorWrapper.h"

#include "ClearableWidget.h"

enum class CalculatorName
{
    Common, Opencl
};


class ModelingScreen : public ClearableWidget
{
    Q_OBJECT

public:
    ModelingScreen(QWidget *parent = nullptr);
    ~ModelingScreen();


public slots:
    void Cleanup() override;


private slots:
    void OpenFile();
    void Compute();
    void SwitchModelMode();
    void SwitchComputeDevice();
    void ImageChanged(QString name);
    void ZoneChanged(QString name);
    void ComputeFinished(CalculatorMode mode, int batchStart, int end);
    bool IsCalculate();
    void SetBatchSize(int value);


private:
    SceneView* _sceneView;
    CodeEditor* _codeEditor;

    QMap<CalculatorName, QThread*> _calculators;
    ISpaceCalculator* _activeCalculator;
    CalculatorName _currentCalculatorName;

    Parser _parser;
    Program* _program;
    std::vector<ArgumentExpr*> _prevArguments;

    int _currentZone;
    int _currentImage;

    QLabel* _modelLabel;
    QLabel* _imageLabel;
    ToggleButton* _imageModeButton;

    QLabel* _computeDevice1;
    QLabel* _computeDevice2;
    ToggleButton* _computeDevice;

    QComboBox* _modelZone;
    QComboBox* _imageType;
    QSpinBox* _spaceDepth;
    QSlider* _batchSize;
    QSpinBox* _batchSizeView;
    QProgressBar* _progressBar;

    QElapsedTimer _timer;

    QCheckBox* _useView;
    QString _filenameBuffer;
    QFile _fileBuffer;
};

#endif // MODELINGSCREEN_H
