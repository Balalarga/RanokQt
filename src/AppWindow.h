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
    void ComputeLine(QString line);
    void ComputeFinished(CalculatorMode mode, int start, int end);
    void StopCalculators();
    bool IsCalculate();
    void SaveData();
    void SetBatchSize(int value);

private:
    Mode m_mode;
    QToolBar* m_toolBar;

    SceneView* m_sceneView;

    CodeEditor* m_codeEditor;
    LineEditor* m_lineEditor;
    QMap<CalculatorName, QSpaceCalculatorWrapper*> _calculators;
    QSpaceCalculatorWrapper* _activeCalculator;

    Parser m_parser;
    Program* m_program;
    Parser m_lineParser;
    Program* m_lineProgram;

    int _currentZone;
    int _currentImage;

    QLabel* _modelLabel;
    QLabel* _imageLabel;
    ToggleButton* m_editorModeButton;

    QLabel* _editorMode1Label;
    QLabel* _editorMode2Label;
    ToggleButton* m_imageModeButton;

    QLabel* _computeDevice1;
    QLabel* _computeDevice2;
    ToggleButton* m_computeDevice;

    QComboBox* _modelZone;
    QStringListModel* _modelZoneModel;
    QComboBox* _imageType;
    QStringListModel* _imageTypeModel;
    QSpinBox* _spaceDepth;
    QSlider* _batchSize;
    QSpinBox* _batchSizeView;
    QProgressBar* _progressBar;

    QElapsedTimer* _timer;


    QPushButton* m_addLineButton;

};
#endif // WIDGET_H
