#include "AppWindow.h"

#include "Gui/StyleLoader.h"

#include "Space/SpaceBuilder.h"
#include "Space/Calculators/CommonCalculator.h"
#include "Space/Calculators/OpenclCalculator.h"

#include <QDebug>
#include <QFileDialog>
#include <QMenuBar>
#include <QStringListModel>
#include <QMessageBox>

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
      _modelZone(new QComboBox(this)),
      _imageType(new QComboBox(this)),
      _spaceDepth(new QSpinBox(this)),
      _batchSize(new QSlider(this)),
      _batchSizeView(new QSpinBox(this)),
      _currentZone(0),
      _currentImage(0),
      _progressBar(new QProgressBar(m_sceneView)),
      _timer(new QElapsedTimer())
{
    QVBoxLayout* m_toolVLayout = new QVBoxLayout(this);
    m_toolVLayout->addWidget(m_toolBar);

    m_toolBar->addAction(QPixmap("assets/images/playIcon.svg"),
                         "Run", this, &AppWindow::Compute);
    m_toolBar->addAction(QPixmap("assets/images/saveIcon.png"),
                         "Save", this, &AppWindow::SaveData);

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
    batchLayout->addWidget(_batchSizeView);


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

    _modelZone->setEditable(false);
    QStringList modelZones;
    modelZones<<"Отрицательная"<<"Нулевая"<<"Положительная";
    _modelZoneModel = new QStringListModel(modelZones, this);
    _modelZone->setModel(_modelZoneModel);
    _modelZone->setCurrentIndex(1);
    modeLayout->addWidget(_modelZone);

    _spaceDepth->setRange(1, 10);
    _spaceDepth->setValue(4);

    _batchSize->setOrientation(Qt::Horizontal);
    _batchSize->setRange(0, 21);
    _batchSize->setValue(0);

    _batchSizeView->setReadOnly(true);
    _batchSizeView->setRange(_batchSize->minimum(), pow(2, _batchSize->maximum()));
    _batchSizeView->setMinimumWidth(100);
    connect(_batchSize, &QSlider::valueChanged, this, &AppWindow::SetBatchSize);

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


    _progressBar->setRange(0, 100);
    _progressBar->setValue(0);


    _calculators[CalculatorName::Common] = new CommonCalculator(this);
    _calculators[CalculatorName::Opencl] = new OpenclCalculator(this);
    for(auto& i: _calculators)
    {
        connect(i, &OpenclCalculator::ComputedModel, this, &AppWindow::ModelComputeFinished);
        connect(i, &OpenclCalculator::ComputedMimage, this, &AppWindow::MimageComputeFinished);
    }

    StyleLoader::attach("../assets/styles/dark.qss");
    m_codeEditor->AddFile("../examples/NewFuncs/lopatka.txt");
    m_codeEditor->AddFile("../examples/NewFuncs/Bone.txt");
    m_codeEditor->AddFile("../examples/NewFuncs/Chainik.txt");

    connect(_imageType, &QComboBox::currentTextChanged, this, &AppWindow::ImageChanged);
    connect(_modelZone, &QComboBox::currentTextChanged, this, &AppWindow::ZoneChanged);
    connect(m_editorModeButton, &QPushButton::clicked, this, &AppWindow::SwitchEditorMode);
    connect(m_imageModeButton, &QPushButton::clicked, this, &AppWindow::SwitchModelMode);
    connect(m_computeDevice, &QPushButton::clicked, this, &AppWindow::SwitchComputeDevice);
    connect(m_addLineButton, &QPushButton::clicked, m_lineEditor, &LineEditor::addItem);
}


AppWindow::~AppWindow()
{
    if(IsCalculate())
    {
        for(auto& i: _calculators)
            i->wait();
    }
    StopCalculators();
    delete _timer;
}

void AppWindow::Compute()
{
    if(IsCalculate())
    {
        StopCalculators();
        return;
    }

    QString source = m_codeEditor->GetActiveText();
    if(!source.isEmpty())
    {
        _progressBar->setValue(0);
        m_parser.SetText(source.toStdString());
        if(m_program)
            delete m_program;
        m_program = m_parser.GetProgram();
        m_sceneView->ClearObjects();
        auto args = m_program->GetSymbolTable().GetAllArgs();
        auto space = SpaceBuilder::Instance().CreateSpace(args[0]->limits, args[1]->limits,
                args[2]->limits, _spaceDepth->value());
        m_sceneView->CreateVoxelObject(space->GetSize());

        _activeCalculator = m_computeDevice->isChecked() ?
                    _calculators[CalculatorName::Opencl] :
                _calculators[CalculatorName::Common];

        _activeCalculator->SetCalculatorMode(m_imageModeButton->isChecked() ?
                                          CalculatorMode::Mimage: CalculatorMode::Model);

        _activeCalculator->SetBatchSize(_batchSizeView->value());
        _activeCalculator->SetProgram(m_program);
        _timer->start();
        _activeCalculator->start();
        qDebug()<<"Start";
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
        _modelZone->setVisible(false);
    }
    else
    {
        // model mode
        _modelLabel->setStyleSheet("QLabel { color : #ffffff; }");
        _imageLabel->setStyleSheet("QLabel { color : #888888; }");
        _imageType->setVisible(false);
        _modelZone->setVisible(true);
    }
}

void AppWindow::SwitchComputeDevice()
{
    if(m_computeDevice->isChecked())
    {
        // Gpu
        _computeDevice1->setStyleSheet("QLabel { color : #888888; }");
        _computeDevice2->setStyleSheet("QLabel { color : #ffffff; }");
    }
    else
    {
        // Cpu
        _computeDevice1->setStyleSheet("QLabel { color : #ffffff; }");
        _computeDevice2->setStyleSheet("QLabel { color : #888888; }");
    }
}

void AppWindow::ImageChanged(QString name)
{
    if(name == "Cx")
        _currentImage = 0;
    else if(name == "Cy")
        _currentImage = 1;
    else if(name == "Cz")
        _currentImage = 2;
    else if(name == "Cw")
        _currentImage = 3;
    else if(name == "Ct")
        _currentImage = 4;

    if(SpaceBuilder::Instance().GetSpace() &&
            SpaceBuilder::Instance().GetSpace()->mimageData)
    {
        auto size = SpaceBuilder::Instance().GetSpace()->GetSize();
        m_sceneView->ClearObjects();
        m_sceneView->CreateVoxelObject(size);
        MimageComputeFinished(0, size);
    }
}

void AppWindow::ZoneChanged(QString name)
{
    if(name == "Отрицательная")
        _currentZone = -1;
    else if(name == "Нулевая")
        _currentZone = 0;
    else if(name == "Положительная")
        _currentZone = 1;

    if(SpaceBuilder::Instance().GetSpace() &&
            SpaceBuilder::Instance().GetSpace()->zoneData)
    {
        auto size = SpaceBuilder::Instance().GetSpace()->GetSize();
        m_sceneView->ClearObjects();
        m_sceneView->CreateVoxelObject(size);
        ModelComputeFinished(0, size);
    }
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
//        m_modelThread->SetProgram(m_lineProgram);
//        m_sceneView->ClearObjects();
//        m_modelThread->start();
    }
}

void AppWindow::ModelComputeFinished(int start, int count)
{
    auto space = SpaceBuilder::Instance().GetSpace();

    int zone = 0;
    QColor modelColor = _activeCalculator->GetModelColor();
    for(; start < count; ++start)
    {
        cl_float3 point = space->GetPos(start);
        zone = space->zoneData->At(start);
        if(zone == _currentZone)
            m_sceneView->AddObject(point.x, point.y, point.z,
                                   modelColor.redF(), modelColor.greenF(),
                                   modelColor.blueF(), modelColor.alphaF());
    }
    m_sceneView->Flush();
    int percent = 100.f*start/space->GetSize();
    _progressBar->setValue(percent);
    if(start == space->GetSize())
        QMessageBox::information(this, "Расчет окончен", "Время расчета = "+QString::number(_timer->restart()/1000.f)+"s");
}

void AppWindow::MimageComputeFinished(int start, int count)
{
    auto space = SpaceBuilder::Instance().GetSpace();
    double value = 0;
    cl_float3 point;
    for(; start < count; ++start)
    {
        point = space->GetPos(start);
        if(_currentImage == 0)
            value = space->mimageData->At(start).Cx;
        else if(_currentImage == 1)
            value = space->mimageData->At(start).Cy;
        else if(_currentImage == 2)
            value = space->mimageData->At(start).Cz;
        else if(_currentImage == 3)
            value = space->mimageData->At(start).Cw;
        else if(_currentImage == 4)
            value = space->mimageData->At(start).Ct;

        QColor color = _activeCalculator->GetMImageColor(value);
        m_sceneView->AddObject(point.x, point.y, point.z,
                               color.redF(), color.greenF(),
                               color.blueF(), color.alphaF());
    }
    m_sceneView->Flush();
    int percent = 100.f*start/space->GetSize();
    _progressBar->setValue(percent);
    if(start == space->GetSize())
        QMessageBox::information(this, "Расчет окончен", "Время расчета = "+QString::number(_timer->restart()));
}

void AppWindow::StopCalculators()
{
    for(auto& i: _calculators)
    {
        if(i->isRunning())
        {
            i->terminate();
            qDebug()<<"StopCalculator";
        }
    }
}

bool AppWindow::IsCalculate()
{
    for(auto& i: _calculators)
    {
        if(i->isRunning())
            return true;
    }
    return false;
}

void AppWindow::SaveData()
{
    if(!SpaceBuilder::Instance().GetSpace())
    {
        QMessageBox::information(this, "Nothing to save",
                                 "You must calculate model or mimage first");
        return;
    }

    if(SpaceBuilder::Instance().GetSpace()->zoneData)
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save model"), "",
            tr("Binary (.mranok)"));
        if (fileName.isEmpty())
            return;

        if(!fileName.endsWith(".mranok"))
            fileName += ".mranok";
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        auto space = SpaceBuilder::Instance().GetSpace();

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_5);
        out << space->startPoint.x;
        out << space->startPoint.y;
        out << space->startPoint.z;
        out << space->pointSize.x;
        out << space->pointSize.y;
        out << space->pointSize.z;
        out << space->spaceUnits.x;
        out << space->spaceUnits.y;
        out << space->spaceUnits.z;
        out.writeBytes((const char*)space->zoneData, space->GetSize()*sizeof(int));
        file.close();
    }
    else if(SpaceBuilder::Instance().GetSpace()->mimageData)
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save MImage"), "",
            tr("Binary (.iranok)"));
        if (fileName.isEmpty())
            return;

        if(!fileName.endsWith(".iranok"))
            fileName += ".iranok";
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        auto space = SpaceBuilder::Instance().GetSpace();

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_5);
        out << space->startPoint.x;
        out << space->startPoint.y;
        out << space->startPoint.z;
        out << space->pointSize.x;
        out << space->pointSize.y;
        out << space->pointSize.z;
        out << space->spaceUnits.x;
        out << space->spaceUnits.y;
        out << space->spaceUnits.z;
        out.writeBytes((const char*)space->mimageData, space->GetSize()*5*sizeof(double));
        file.close();
    }
}

void AppWindow::SetBatchSize(int value)
{
    _batchSizeView->setValue(pow(2, value));
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
