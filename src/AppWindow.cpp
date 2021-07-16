#include "AppWindow.h"

#include "Gui/StyleLoader.h"

#include "Space/SpaceBuilder.h"
#include "OpenclCalculator.h"

#include <iostream>

#include <QDebug>
#include <QFileDialog>
#include <QMenuBar>
#include <QStringListModel>

AppWindow::AppWindow(QWidget *parent)
    : QWidget(parent),
      m_mode(Mode::Common),
      m_toolBar(new QToolBar(this)),
      m_sceneView(new SceneView(this)),
      m_codeEditor(new CodeEditor(this)),
      m_lineEditor(new LineEditor(this)),
      m_program(nullptr),
      m_lineProgram(nullptr),
      m_editorModeButton(new ToggleButton(8, 10, this)),
      m_imageModeButton(new ToggleButton(8, 10, this)),
      m_computeDevice(new ToggleButton(8, 10, this)),
      m_addLineButton(new QPushButton("Добавить строку", this)),
      _imageType(new QComboBox(this)),
      _spaceDepth(new QSpinBox(this)),
      _batchSize(new QSpinBox(this)),
      _currentZone(0),
      _currentType(0)
{
    QVector<QColor> gradColors;
    gradColors.push_back(QColor(255, 255, 0, 5));
    gradColors.push_back(QColor(0, 255, 162, 5));
    gradColors.push_back(QColor(0, 0, 255, 5));
    gradColors.push_back(QColor(255, 145, 0, 5));
    gradColors.push_back(QColor(214, 0, 255, 5));
    _linearGradModel = new LinearGradientModel(gradColors, this);

    QVBoxLayout* m_toolVLayout = new QVBoxLayout(this);
    m_toolVLayout->addWidget(m_toolBar);

    m_toolBar->addAction(QPixmap("assets/images/playIcon.svg"),
                         "Run", this, &AppWindow::Compute);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    QWidget* wrapWidget = new QWidget(this);
    QVBoxLayout* modeLayout = new QVBoxLayout(wrapWidget);
    QHBoxLayout* spinLayout = new QHBoxLayout();
    QLabel* spinLabel = new QLabel("Глубина рекурсии");
    spinLayout->addWidget(spinLabel);
    spinLayout->addWidget(_spaceDepth);

    QHBoxLayout* batchLayout = new QHBoxLayout();
    QLabel* batchLabel = new QLabel("Размер пачки");
    batchLayout->addWidget(batchLabel);
    batchLayout->addWidget(_batchSize);


    modeLayout->addLayout(spinLayout);
    modeLayout->addLayout(batchLayout);
    modeLayout->addWidget(m_codeEditor);
    modeLayout->addWidget(m_lineEditor);
    modeLayout->addWidget(m_addLineButton);

    QHBoxLayout* editorModeBtnLayout = new QHBoxLayout;
    editorModeBtnLayout->setContentsMargins(0, 10, 0, 0);
    _editorMode1Label = new QLabel("Обычный режим");
    _editorMode1Label->setAlignment(Qt::AlignVCenter);
    _editorMode1Label->setAlignment(Qt::AlignRight);
    _editorMode1Label->setStyleSheet("QLabel { color : #ffffff; }");
    _editorMode2Label = new QLabel("Построчный режим");
    _editorMode2Label->setStyleSheet("QLabel { color : #888888; }");
    _editorMode2Label->setAlignment(Qt::AlignVCenter);
    editorModeBtnLayout->addWidget(_editorMode1Label);
    editorModeBtnLayout->addWidget(m_editorModeButton);
    editorModeBtnLayout->addWidget(_editorMode2Label);
    modeLayout->addLayout(editorModeBtnLayout);

    QHBoxLayout* modelModeBtnLayout = new QHBoxLayout;
    modelModeBtnLayout->setContentsMargins(0, 10, 0, 0);
    _modelLabel = new QLabel("Модель");
    _modelLabel->setAlignment(Qt::AlignVCenter);
    _modelLabel->setAlignment(Qt::AlignRight);
    _modelLabel->setStyleSheet("QLabel { color : #ffffff; }");
    _imageLabel = new QLabel("М-Образ");
    _imageLabel->setStyleSheet("QLabel { color : #888888; }");
    _imageLabel->setAlignment(Qt::AlignVCenter);
    modelModeBtnLayout->addWidget(_modelLabel);
    modelModeBtnLayout->addWidget(m_imageModeButton);
    modelModeBtnLayout->addWidget(_imageLabel);
    modeLayout->addLayout(modelModeBtnLayout);


    QHBoxLayout* deviceModeLayout = new QHBoxLayout;
    deviceModeLayout->setContentsMargins(0, 10, 0, 0);
    _computeDevice1 = new QLabel("CPU");
    _computeDevice1->setAlignment(Qt::AlignVCenter);
    _computeDevice1->setAlignment(Qt::AlignRight);
    _computeDevice1->setStyleSheet("QLabel { color : #ffffff; }");
    _computeDevice2 = new QLabel("GPU");
    _computeDevice2->setStyleSheet("QLabel { color : #888888; }");
    _computeDevice2->setAlignment(Qt::AlignVCenter);
    deviceModeLayout->addWidget(_computeDevice1);
    deviceModeLayout->addWidget(m_computeDevice);
    deviceModeLayout->addWidget(_computeDevice2);
    modeLayout->addLayout(deviceModeLayout);


    _imageType->setEditable(false);
    QStringList imageNames;
    imageNames<<"Cx"<<"Cy"<<"Cz"<<"Cw"<<"Ct";
    _imageTypeModel = new QStringListModel(imageNames, this);
    _imageType->setModel(_imageTypeModel);
    modeLayout->addWidget(_imageType);
    _imageType->setVisible(false);

    _spaceDepth->setRange(1, 10);
    _spaceDepth->setValue(4);

    _batchSize->setRange(0, 10000);
    _batchSize->setValue(0);

    m_addLineButton->setVisible(false);
    wrapWidget->setLayout(modeLayout);
    m_lineEditor->setVisible(false);

    splitter->addWidget(wrapWidget);
    splitter->addWidget(m_sceneView);
    splitter->setMidLineWidth(2);
    m_sceneView->setMinimumWidth(500);
    m_toolVLayout->addWidget(splitter);

    QMenuBar* menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("Файл");
    menuBar->addMenu(fileMenu);

    QAction* saveAction = new QAction;
    saveAction->setText("Открыть");
    connect(saveAction, &QAction::triggered, this, &AppWindow::OpenFile);
    fileMenu->addAction(saveAction);

    m_toolVLayout->setMenuBar(menuBar);

    m_imageThread = new ImageThread(this);
    m_modelThread = new ModelThread(this);

    StyleLoader::attach("../assets/styles/dark.qss");
    m_codeEditor->AddFile("../examples/NewFuncs/sphere.txt");

    connect(&SpaceCalculator::Get(), &SpaceCalculator::ComputedModel, this, &AppWindow::ModelComputeFinished);
    connect(&SpaceCalculator::Get(), &SpaceCalculator::ComputedMimage, this, &AppWindow::MimageComputeFinished);
    connect(&OpenclCalculator::Get(), &OpenclCalculator::ComputedModel, this, &AppWindow::ModelComputeFinished);
    connect(&OpenclCalculator::Get(), &OpenclCalculator::ComputedMimage, this, &AppWindow::MimageComputeFinished);

    connect(_imageType, &QComboBox::currentTextChanged, this, &AppWindow::ImageChanged);
    connect(m_editorModeButton, &QPushButton::clicked, this, &AppWindow::SwitchEditorMode);
    connect(m_imageModeButton, &QPushButton::clicked, this, &AppWindow::SwitchModelMode);
    connect(m_computeDevice, &QPushButton::clicked, this, &AppWindow::SwitchComputeDevice);
    connect(m_addLineButton, &QPushButton::clicked, m_lineEditor, &LineEditor::addItem);
    connect(m_lineEditor, &LineEditor::runLine, this, &AppWindow::ComputeLine);
}


AppWindow::~AppWindow()
{
    if(m_modelThread &&
            m_modelThread->isRunning())
        m_modelThread->terminate();
    if(m_imageThread &&
            m_imageThread->isRunning())
        m_imageThread->terminate();
}

#include <iostream>
using namespace std;
void AppWindow::Compute()
{
    QString source = m_codeEditor->GetActiveText();
    if(!source.isEmpty())
    {
        if(m_modelThread->isRunning() ||
                m_imageThread->isRunning())
        {
            return;
        }
        m_parser.SetText(source.toStdString());
        if(m_program)
            delete m_program;
        m_program = m_parser.GetProgram();
        m_sceneView->ClearObjects();
        auto args = m_program->GetSymbolTable().GetAllArgs();
        auto space = SpaceBuilder::Instance().CreateSpace(args[0]->limits, args[1]->limits,
                args[2]->limits, _spaceDepth->value());
        m_sceneView->CreateVoxelObject(space->GetSize());

        if(m_imageModeButton->isChecked())
        {
            m_modelThread->SetBatchSize(_batchSize->value());
            m_imageThread->SetProgram(m_program);
            m_imageThread->start();
        }
        else
        {
            m_modelThread->SetBatchSize(_batchSize->value());
            m_modelThread->SetProgram(m_program);
            m_modelThread->start();
        }
    }
}


void AppWindow::SwitchEditorMode()
{
    if(m_mode == Mode::Common)
    {
        m_codeEditor->setVisible(false);
        m_lineEditor->setVisible(true);
        m_addLineButton->setVisible(true);
        m_mode = Mode::Line;
        //        m_modeButton->setText("Построчный режим");
        _editorMode1Label->setStyleSheet("QLabel { color : #888888; }");
        _editorMode2Label->setStyleSheet("QLabel { color : #ffffff; }");
    }
    else
    {
        m_codeEditor->setVisible(true);
        m_lineEditor->setVisible(false);
        m_addLineButton->setVisible(false);
        m_mode = Mode::Common;
        //        m_modeButton->setText("Обычный режим");
        _editorMode1Label->setStyleSheet("QLabel { color : #ffffff; }");
        _editorMode2Label->setStyleSheet("QLabel { color : #888888; }");
    }
}

void AppWindow::SwitchModelMode()
{
    if(m_imageModeButton->isChecked())
    {
        // image mode
        _modelLabel->setStyleSheet("QLabel { color : #888888; }");
        _imageLabel->setStyleSheet("QLabel { color : #ffffff; }");
        _imageType->setVisible(true);
    }
    else
    {
        // model mode
        _modelLabel->setStyleSheet("QLabel { color : #ffffff; }");
        _imageLabel->setStyleSheet("QLabel { color : #888888; }");
        _imageType->setVisible(false);
    }
}

void AppWindow::SwitchComputeDevice()
{
    if(m_computeDevice->isChecked())
    {
        // Gpu
        _computeDevice1->setStyleSheet("QLabel { color : #888888; }");
        _computeDevice2->setStyleSheet("QLabel { color : #ffffff; }");
        m_modelThread->SetComputeMode(ComputeMode::Gpu);
        m_imageThread->SetComputeMode(ComputeMode::Gpu);
    }
    else
    {
        // Cpu
        _computeDevice1->setStyleSheet("QLabel { color : #ffffff; }");
        _computeDevice2->setStyleSheet("QLabel { color : #888888; }");
        m_modelThread->SetComputeMode(ComputeMode::Cpu);
        m_imageThread->SetComputeMode(ComputeMode::Cpu);
    }
}

void AppWindow::ImageChanged(QString name)
{
    if(name == "Cx")
        _currentType = 0;
    else if(name == "Cy")
        _currentType = 1;
    else if(name == "Cz")
        _currentType = 2;
    else if(name == "Cw")
        _currentType = 3;
    else if(name == "Ct")
        _currentType = 4;
}

void AppWindow::ComputeLine(QString line)
{
    m_lineParser.SetText(line.toStdString());

    if(m_lineProgram)
    {
        Program* lineProg = m_lineParser.GetProgram(&m_lineProgram->GetSymbolTable());
        if(auto res = m_lineProgram->MergeProgram(lineProg))
        {
            m_lineProgram->SetResult(res);
            delete lineProg;
        }
        else
        {
            qDebug()<<"Program merging error:"<<line;
            return;
        }
    }
    else
    {
        m_lineProgram = m_lineParser.GetProgram();
    }

    if(!m_lineProgram->GetSymbolTable().GetAllArgs().empty())
    {
        m_modelThread->SetProgram(m_lineProgram);
        m_sceneView->ClearObjects();
        m_modelThread->start();
    }
}

void AppWindow::ModelComputeFinished(int start, int end)
{
    auto space = SpaceBuilder::Instance().GetSpace();

    for(int i = start; i < end; ++i)
    {
        cl_float3 point = space->GetPos(i);
        if(space->zoneData->At(i) == _currentZone)
            m_sceneView->AddObject(point.x, point.y, point.z);
    }
    m_sceneView->Flush();
}

void AppWindow::MimageComputeFinished(int start, int end)
{
    auto space = SpaceBuilder::Instance().GetSpace();
    for(int i = start; i < end; ++i)
    {
        cl_float3 point = space->GetPos(i);
        double value = space->mimageData->At(i).Cx;
        value = (1. + value)/2.;
        unsigned uValue = UINT_MAX*value;
        QColor color = _linearGradModel->GetColor(uValue);
        m_sceneView->AddObject(point.x, point.y, point.z);
    }
    m_sceneView->Flush();
}

void AppWindow::OpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open program file"), "../",
                                                    tr("Plan text(*.txt)"));
    if(!fileName.isEmpty())
        m_codeEditor->AddFile(fileName);
}

bool AppWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(QKeySequence("F6") == QKeySequence(keyEvent->key()))
        {
            QString fileName = QFileDialog::getOpenFileName(this,
                                                            tr("Stylesheet"), "../style",
                                                            tr("Stylesheet(*.qss)"));
            if(!fileName.isEmpty())
            {
                StyleLoader::attach(fileName);
            }
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}
