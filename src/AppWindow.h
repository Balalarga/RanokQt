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
#include <QLabel>
#include <QMap>
#include <QSlider>
#include <QProgressBar>
#include <QElapsedTimer>
#include <QQueue>

#include "Gui/ToggleButton.h"

#include "Gui/Opengl/SceneView.h"
#include "Gui/CodeEditor/CodeEditor.h"
#include "Gui/CodeEditor/LineEditor.h"

#include "Language/Parser.h"
#include "QSpaceCalculatorWrapper.h"

enum class Mode
{
    Common, Line
};
enum class CalculatorName
{
    Common, Opencl
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
    void SwitchComputeDevice();
    void ImageChanged(QString name);
    void ZoneChanged(QString name);
    void ComputeLine(int id, QString line);
    void ComputeFinished(CalculatorMode mode, int start, int batchStart, int end);
    void StopCalculators();
    bool IsCalculate();
    void SaveData();
    void SetBatchSize(int value);

private:
    int _bufferSize;

    Mode _mode;
    QToolBar* _toolBar;

    SceneView* _sceneView;

    CodeEditor* _codeEditor;
    LineEditor* _lineEditor;
    QMap<CalculatorName, QThread*> _calculators;
    ISpaceCalculator* _activeCalculator;
    CalculatorName _currentCalculatorName;

    Parser _parser;
    Program* _program;
    Program* _singleLineProgram;
    std::vector<ArgumentExpr*> _prevArguments;

    int _currentZone;
    int _currentImage;

    QLabel* _modelLabel;
    QLabel* _imageLabel;
    ToggleButton* m_editorModeButton;

    QLabel* _editorMode1Label;
    QLabel* _editorMode2Label;
    ToggleButton* _imageModeButton;

    QLabel* _computeDevice1;
    QLabel* _computeDevice2;
    ToggleButton* _computeDevice;

    QComboBox* _modelZone;
    QStringListModel* _modelZoneModel;
    QComboBox* _imageType;
    QStringListModel* _imageTypeModel;
    QSpinBox* _spaceDepth;
    QLabel* _batchLabel;
    QSlider* _batchSize;
    QSpinBox* _batchSizeView;
    QProgressBar* _progressBar;

    QElapsedTimer* _timer;


    QPushButton* _addLineButton;
    QQueue<Color> _linePallet;

};
#endif // WIDGET_H
