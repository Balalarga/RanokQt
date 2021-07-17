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

#include "Gui/ToggleButton.h"

#include "Gui/Opengl/SceneView.h"
#include "Gui/CodeEditor/CodeEditor.h"
#include "Gui/CodeEditor/LineEditor.h"

#include "Language/Parser.h"
#include "Space/Calculators/ISpaceCalculator.h"

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
    void ComputeLine(QString line);
    void ModelComputeFinished(int start, int end);
    void MimageComputeFinished(int start, int end);
    void StopCalculators();
    bool IsCalculate();

private:
    Mode m_mode;
    QToolBar* m_toolBar;

    SceneView* m_sceneView;

    CodeEditor* m_codeEditor;
    LineEditor* m_lineEditor;
    QMap<CalculatorName, ISpaceCalculator*> _calculators;
    ISpaceCalculator* _activeCalculator;

    Parser m_parser;
    Program* m_program;
    Parser m_lineParser;
    Program* m_lineProgram;

    int _currentZone;
    int _currentType;

    QLabel* _editorMode1Label;
    QLabel* _editorMode2Label;
    ToggleButton* m_editorModeButton;

    QLabel* _modelLabel;
    QLabel* _imageLabel;
    ToggleButton* m_imageModeButton;

    QLabel* _computeDevice1;
    QLabel* _computeDevice2;
    ToggleButton* m_computeDevice;

    QComboBox* _imageType;
    QStringListModel* _imageTypeModel;
    QSpinBox* _spaceDepth;
    QSpinBox* _batchSize;


    QPushButton* m_addLineButton;
};
#endif // WIDGET_H
