#include "TestScreen.h"

#include "Space/SpaceManager.h"
#include "Space/Calculators/CommonCalculator.h"
#include "Space/Calculators/OpenclCalculator.h"

#include <QDebug>
#include <QFileDialog>
#include <QMenuBar>
#include <QStringListModel>
#include <QMessageBox>
#include <fstream>


TestScreen::TestScreen(QWidget *parent)
    : ClearableWidget(parent),
      _sceneView(new TestView(this)),
      _codeEditor(new CodeEditor(this)),
      _program(nullptr),
      _progressBar(new QProgressBar(_sceneView))
{
    QVBoxLayout* toolVLayout = new QVBoxLayout(this);

    QToolBar* _toolBar(new QToolBar(this));
    _toolBar->addAction(QPixmap("assets/images/playIcon.svg"),
                        "Run", this, &TestScreen::Compute);
    toolVLayout->addWidget(_toolBar);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    QWidget* wrapWidget = new QWidget(this);
    QVBoxLayout* modeLayout = new QVBoxLayout(wrapWidget);

    modeLayout->addWidget(_codeEditor);

    wrapWidget->setLayout(modeLayout);

    splitter->addWidget(wrapWidget);
    splitter->addWidget(_sceneView);
    splitter->setMidLineWidth(2);
    _sceneView->setMinimumWidth(500);
    toolVLayout->addWidget(splitter);

    QMenuBar* menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("Файл");
    menuBar->addMenu(fileMenu);

    QAction* saveAction = new QAction;
    saveAction->setText("Открыть");
    connect(saveAction, &QAction::triggered, this, &TestScreen::OpenFile);
    fileMenu->addAction(saveAction);

    toolVLayout->setMenuBar(menuBar);

    _progressBar->setRange(0, 100);
    _progressBar->setValue(0);

    _codeEditor->AddFile("../Core/Examples/NewFuncs/Bone.txt");
    _codeEditor->AddFile("../Core/Examples/NewFuncs/sphere.txt");
    _oldTabId = _codeEditor->currentIndex();
}

TestScreen::~TestScreen()
{

}

void TestScreen::Cleanup()
{

}

void TestScreen::OpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open program file"), "../",
                                                    tr("Plan text(*.txt)"));
    if(!fileName.isEmpty())
        _codeEditor->AddFile(fileName);
}

void TestScreen::Compute()
{

}
