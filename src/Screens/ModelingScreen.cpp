#include "ModelingScreen.h"

#include "Gui/StyleLoader.h"

#include "Space/SpaceManager.h"
#include "Space/Calculators/CommonCalculator.h"
#include "Space/Calculators/OpenclCalculator.h"

#include <QDebug>
#include <QFileDialog>
#include <QMenuBar>
#include <QStringListModel>
#include <QMessageBox>
#include <fstream>


ModelingScreen::ModelingScreen(QWidget *parent)
    : QWidget(parent),
      _sceneView(new SceneView(this)),
      _codeEditor(new CodeEditor(this)),
      _program(nullptr),
      _imageModeButton(new ToggleButton(8, 10, this)),
      _computeDevice(new ToggleButton(8, 10, this)),
      _modelZone(new QComboBox(this)),
      _imageType(new QComboBox(this)),
      _spaceDepth(new QSpinBox(this)),
      _batchSize(new QSlider(this)),
      _batchSizeView(new QSpinBox(this)),
      _currentZone(0),
      _currentImage(0),
      _currentCalculatorName(CalculatorName::Common),
      _progressBar(new QProgressBar(_sceneView))
{
    QVBoxLayout* toolVLayout = new QVBoxLayout(this);

    QToolBar* _toolBar(new QToolBar(this));
    _toolBar->addAction(QPixmap("assets/images/playIcon.svg"),
                         "Run", this, &ModelingScreen::Compute);
    _toolBar->addAction(QPixmap("assets/images/saveIcon.png"),
                         "Save", this, &ModelingScreen::SaveData);
    toolVLayout->addWidget(_toolBar);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    QWidget* wrapWidget = new QWidget(this);
    QVBoxLayout* modeLayout = new QVBoxLayout(wrapWidget);
    QHBoxLayout* spinLayout = new QHBoxLayout();
    QLabel* spinLabel = new QLabel("Глубина рекурсии");
    spinLayout->addWidget(spinLabel);
    spinLayout->addWidget(_spaceDepth);

    QHBoxLayout* batchLayout = new QHBoxLayout();
    QLabel* _batchLabel = new QLabel("Размер буффера");
    batchLayout->addWidget(_batchLabel);
    batchLayout->addWidget(_batchSize);
    batchLayout->addWidget(_batchSizeView);


    modeLayout->addLayout(spinLayout);
    modeLayout->addLayout(batchLayout);
    modeLayout->addWidget(_codeEditor);

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
    modelModeBtnLayout->addWidget(_imageModeButton);
    modelModeBtnLayout->addWidget(_imageLabel);
    modeLayout->addLayout(modelModeBtnLayout);


    QHBoxLayout* deviceModeLayout = new QHBoxLayout;
    deviceModeLayout->setContentsMargins(0, 10, 0, 0);
    _computeDevice1 = new QLabel("Медленно");
    _computeDevice1->setAlignment(Qt::AlignVCenter);
    _computeDevice1->setAlignment(Qt::AlignRight);
    _computeDevice1->setStyleSheet("QLabel { color : #ffffff; }");
    _computeDevice2 = new QLabel("Быстро");
    _computeDevice2->setStyleSheet("QLabel { color : #888888; }");
    _computeDevice2->setAlignment(Qt::AlignVCenter);
    deviceModeLayout->addWidget(_computeDevice1);
    deviceModeLayout->addWidget(_computeDevice);
    deviceModeLayout->addWidget(_computeDevice2);
    modeLayout->addLayout(deviceModeLayout);


    _imageType->setEditable(false);
    QStringList imageNames;
    imageNames<<"Cx"<<"Cy"<<"Cz"<<"Cw"<<"Ct";
    QStringListModel* _imageTypeModel = new QStringListModel(imageNames, this);
    _imageType->setModel(_imageTypeModel);
    modeLayout->addWidget(_imageType);
    _imageType->setVisible(false);

    _modelZone->setEditable(false);
    QStringList modelZones;
    modelZones<<"Отрицательная"<<"Нулевая"<<"Положительная";
    QStringListModel* _modelZoneModel = new QStringListModel(modelZones, this);
    _modelZone->setModel(_modelZoneModel);
    _modelZone->setCurrentIndex(1);
    modeLayout->addWidget(_modelZone);

    _spaceDepth->setRange(1, 10);
    _spaceDepth->setValue(4);

    _batchSize->setOrientation(Qt::Horizontal);
    _batchSize->setRange(0, 29);
    _batchSize->setValue(0);

    _batchSizeView->setReadOnly(true);
    _batchSizeView->setRange(_batchSize->minimum(), pow(2, _batchSize->maximum()));
    _batchSizeView->setMinimumWidth(100);
    connect(_batchSize, &QSlider::valueChanged, _batchSizeView, &QSpinBox::setValue);

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
    connect(saveAction, &QAction::triggered, this, &ModelingScreen::OpenFile);
    fileMenu->addAction(saveAction);

    toolVLayout->setMenuBar(menuBar);


    _progressBar->setRange(0, 100);
    _progressBar->setValue(0);


    CommonCalculatorThread* commonCalculator = new CommonCalculatorThread(this);
    OpenclCalculatorThread* openclCalculator = new OpenclCalculatorThread(this);
    _calculators[CalculatorName::Common] = commonCalculator;
    _calculators[CalculatorName::Opencl] = openclCalculator;

    qRegisterMetaType<CalculatorMode>("CalculatorMode");
    connect(commonCalculator, &CommonCalculatorThread::Computed,
            this, &ModelingScreen::ComputeFinished, Qt::BlockingQueuedConnection);
    connect(openclCalculator , &OpenclCalculatorThread::Computed,
            this, &ModelingScreen::ComputeFinished, Qt::BlockingQueuedConnection);

    StyleLoader::attach("../assets/styles/dark.qss");
    _codeEditor->AddFile("../Core/Examples/NewFuncs/lopatka.txt");
    _codeEditor->AddFile("../Core/Examples/NewFuncs/Bone.txt");
    _codeEditor->AddFile("../Core/Examples/NewFuncs/Chainik.txt");

    connect(_imageType, &QComboBox::currentTextChanged, this, &ModelingScreen::ImageChanged);
    connect(_modelZone, &QComboBox::currentTextChanged, this, &ModelingScreen::ZoneChanged);
    connect(_imageModeButton, &QPushButton::clicked, this, &ModelingScreen::SwitchModelMode);
    connect(_computeDevice, &QPushButton::clicked, this, &ModelingScreen::SwitchComputeDevice);
}

ModelingScreen::~ModelingScreen()
{
    for(auto& i: _calculators)
        if(i->isRunning())
            i->terminate();
}

void ModelingScreen::Compute()
{
    if(IsCalculate())
    {
        return;
    }

    QString source = _codeEditor->GetActiveText();
    if(!source.isEmpty())
    {
        _progressBar->setValue(0);
        _parser.SetText(source.toStdString());
        if(_program)
            delete _program;
        _program = _parser.GetProgram();
        _sceneView->ClearObjects();
        auto args = _program->GetSymbolTable().GetAllArgs();
        if(SpaceManager::ComputeSpaceSize(_spaceDepth->value()) !=
                SpaceManager::Self().GetSpaceSize() ||
                _prevArguments != args)
        {
            _prevArguments = args;
            SpaceManager::Self().InitSpace(args[0]->limits, args[1]->limits,
                    args[2]->limits, _spaceDepth->value());
            if(_batchSizeView->value() != 0)
                SpaceManager::Self().ResetBufferSize(pow(2, _batchSizeView->value()));
            else
                SpaceManager::Self().ResetBufferSize(0);
        }
        _sceneView->CreateVoxelObject(SpaceManager::Self().GetSpaceSize());

        _activeCalculator = dynamic_cast<ISpaceCalculator*>(_computeDevice->isChecked() ?
                    _calculators[CalculatorName::Opencl] :
                _calculators[CalculatorName::Common]);

        _activeCalculator->SetCalculatorMode(_imageModeButton->isChecked() ?
                                          CalculatorMode::Mimage: CalculatorMode::Model);

        _activeCalculator->SetProgram(_program);
        _timer.start();
        _calculators[_currentCalculatorName]->start();
        qDebug()<<"Start";
    }
}


void ModelingScreen::SwitchModelMode()
{
    if(_imageModeButton->isChecked())
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

void ModelingScreen::SwitchComputeDevice()
{
    if(_computeDevice->isChecked())
    {
        // Gpu
        _computeDevice1->setStyleSheet("QLabel { color : #888888; }");
        _computeDevice2->setStyleSheet("QLabel { color : #ffffff; }");
        _currentCalculatorName = CalculatorName::Opencl;
    }
    else
    {
        // Cpu
        _computeDevice1->setStyleSheet("QLabel { color : #ffffff; }");
        _computeDevice2->setStyleSheet("QLabel { color : #888888; }");
        _currentCalculatorName = CalculatorName::Common;
    }
}

void ModelingScreen::ImageChanged(QString name)
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

    if(SpaceManager::Self().WasInited() &&
            SpaceManager::Self().GetMimageBuffer())
    {
        auto size = SpaceManager::Self().GetSpaceSize();
        _sceneView->ClearObjects();
        _sceneView->CreateVoxelObject(size);
        ComputeFinished(CalculatorMode::Mimage, 0, size);
    }
}

void ModelingScreen::ZoneChanged(QString name)
{
    if(name == "Отрицательная")
        _currentZone = -1;
    else if(name == "Нулевая")
        _currentZone = 0;
    else if(name == "Положительная")
        _currentZone = 1;

    if(SpaceManager::Self().WasInited() &&
            SpaceManager::Self().GetZoneBuffer())
    {
        auto size = SpaceManager::Self().GetSpaceSize();
        _sceneView->ClearObjects();
        _sceneView->CreateVoxelObject(size);
        ComputeFinished(CalculatorMode::Model, 0, size);
    }
}

void ModelingScreen::ComputeFinished(CalculatorMode mode, int batchStart, int count)
{
    SpaceManager& space = SpaceManager::Self();

    if(mode == CalculatorMode::Model)
    {
        int zone = 0;
        cl_float3 point;
        Color modelColor;
        modelColor = _activeCalculator->GetModelColor();
        for(int i = 0; i < count; ++i)
        {
            point = space.GetPointCoords(batchStart+i);
            zone = space.GetZone(i);
            if(zone == _currentZone)
                _sceneView->AddObject(point.x, point.y, point.z,
                                       modelColor.red, modelColor.green,
                                       modelColor.blue, modelColor.alpha);
        }
    }
    else
    {
        double value = 0;
        cl_float3 point;
        for(int i = 0; i < count; ++i)
        {
            point = space.GetPointCoords(batchStart+i);
            if(_currentImage == 0)
                value = space.GetMimage(i).Cx;
            else if(_currentImage == 1)
                value = space.GetMimage(i).Cy;
            else if(_currentImage == 2)
                value = space.GetMimage(i).Cz;
            else if(_currentImage == 3)
                value = space.GetMimage(i).Cw;
            else if(_currentImage == 4)
                value = space.GetMimage(i).Ct;

            Color color = _activeCalculator->GetMImageColor(value);
            _sceneView->AddObject(point.x, point.y, point.z,
                                   color.red, color.green,
                                   color.blue, color.alpha);
        }
    }
    _sceneView->Flush();
    int percent = 100.f*(batchStart+count)/space.GetSpaceSize();
    _progressBar->setValue(percent);
    if(percent == 100 && _timer.isValid())
        QMessageBox::about(this, "Расчет окончен", "Время расчета = "+
                                 QString::number(_timer.restart()/1000.f)+"s");
}


bool ModelingScreen::IsCalculate()
{
    for(auto& i: _calculators)
    {
        if(i->isRunning())
            return true;
    }
    return false;
}

void ModelingScreen::SaveData()
{
    if(!SpaceManager::Self().WasInited())
    {
        QMessageBox::about(this, "Nothing to save",
                                 "You must calculate model or mimage first");
        return;
    }
    QString file = QFileDialog::getSaveFileName(this,"","","*.bin");

    if(!file.endsWith(".bin"))
        file += ".bin";

    std::ofstream stream(file.toStdString());
    if(!stream)
    {
        QMessageBox::about(this, "Couldn't open file",
                                 "Error while openin file "+file);
        return;
    }

    if(_activeCalculator->GetCalculatorMode() == CalculatorMode::Model)
        SpaceManager::Self().SaveZoneRange(stream, 0);
    else
        SpaceManager::Self().SaveMimageRange(stream, 0);
    stream.close();

    QMessageBox::about(this, "Saved",
                             "Data saved at "+file);
}

void ModelingScreen::SetBatchSize(int value)
{
    if(value == 0)
        _batchSizeView->setValue(0);
    else
        _batchSizeView->setValue(pow(2, value));

}

void ModelingScreen::OpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open program file"), "../",
                                                    tr("Plan text(*.txt)"));
    if(!fileName.isEmpty())
        _codeEditor->AddFile(fileName);
}
