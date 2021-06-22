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

#include "Calculators/MImageCalculator.h"
#include "Calculators/ModelCalculator.h"
#include "Calculators/MImageMatrixCalculator.h"
#include "Calculators/RecursiveCalculator.h"
#include "Calculators/MatrixCalculator.h"

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


public slots:
    void AddVoxel(VoxelData data);


private slots:
    void OpenFile();
    void Compute();
    void SwitchMode();


private:
    Mode m_mode;
    QToolBar* m_toolBar;

    SceneView* m_sceneView;

    CodeEditor* m_codeEditor;
    LineEditor* m_lineEditor;
    TaskThread* m_execThread;
    ModelCalculator* m_modelCalculator;

    Parser m_parser;
    Program* m_program;
    QPushButton* m_modeButton;
    QPushButton* m_addLineButton;
};
#endif // WIDGET_H
