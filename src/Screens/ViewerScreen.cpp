#include "ViewerScreen.h"

#include <QFileDialog>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QDataStream>

#include "Space/SpaceManager.h"
#include "Space/Calculators/ISpaceCalculator.h"


ViewerScreen::ViewerScreen(QWidget *parent):
    ClearableWidget(parent),
    _view(new SceneView(this)),
    _lowMimageLimiter(new QDoubleSpinBox(this)),
    _highMimageLimiter(new QDoubleSpinBox(this)),
    _xSpaceLimiter(new QDoubleSpinBox(this)),
    _ySpaceLimiter(new QDoubleSpinBox(this)),
    _zSpaceLimiter(new QDoubleSpinBox(this))
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QMenuBar* menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("Файл");
    menuBar->addMenu(fileMenu);

    QAction* openAction = new QAction;
    openAction->setText("Открыть");
    connect(openAction, &QAction::triggered,
            this, &ViewerScreen::OpenFile);
    fileMenu->addAction(openAction);
    mainLayout->setMenuBar(menuBar);

    _lowMimageLimiter->setRange(-1.0, 1.0);
    _lowMimageLimiter->setValue(-1.0);
    _lowMimageLimiter->setSingleStep(0.05);
    _lowMimageLimiter->setVisible(false);
    connect(_lowMimageLimiter, SIGNAL(valueChanged(double)),
            this, SLOT(LowMimageLimiterChanged(double)));

    _highMimageLimiter->setRange(-1.0, 1.0);
    _highMimageLimiter->setValue(1.0);
    _highMimageLimiter->setSingleStep(0.05);
    _highMimageLimiter->setVisible(false);

    connect(_highMimageLimiter, SIGNAL(valueChanged(double)),
            this, SLOT(HighMimageLimiterChanged(double)));
    _xSpaceLimiter->setSingleStep(0.05);

    _ySpaceLimiter->setSingleStep(0.05);

    _zSpaceLimiter->setSingleStep(0.05);

    QHBoxLayout* mimageLimitersLayout = new QHBoxLayout();
    mimageLimitersLayout->addWidget(_lowMimageLimiter);
    mimageLimitersLayout->addWidget(_highMimageLimiter);

    mainLayout->addWidget(_xSpaceLimiter);
    mainLayout->addWidget(_ySpaceLimiter);
    mainLayout->addWidget(_zSpaceLimiter);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(mimageLimitersLayout);
    mainLayout->addWidget(_view);
}

void ViewerScreen::Cleanup()
{
    _view->ClearObjects();
}

void ViewerScreen::OpenFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open file"), "",
                                                    tr("Binary file(*.mbin; *.ibin; *.bin)"));
    if(filePath.isEmpty())
        return;

    if(filePath.endsWith(".mbin"))
    {
        OpenModel(filePath);
    }
    else if(filePath.endsWith(".ibin"))
    {
        OpenMimage(filePath);
    }
}

void ViewerScreen::OpenMimage(const QString &filePath)
{
    disconnect(_xSpaceLimiter, SIGNAL(valueChanged(double)),
               this, SLOT(XSpaceLimiterChanged(double)));
    disconnect(_ySpaceLimiter, SIGNAL(valueChanged(double)),
               this, SLOT(YSpaceLimiterChanged(double)));
    disconnect(_zSpaceLimiter, SIGNAL(valueChanged(double)),
               this, SLOT(ZSpaceLimiterChanged(double)));
    _mode = Mode::Mimage;
    _lowMimageLimiter->setVisible(true);
    _highMimageLimiter->setVisible(true);

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
        return;
    SpaceManager& space = SpaceManager::Self();
    QDataStream stream(&file);

    ModelMetadata metadata;
    stream.readRawData((char*)&metadata, sizeof(ModelMetadata));

    space.SetMetadata(metadata);
    space.InitFromMetadata();
    space.ActivateBuffer(SpaceManager::BufferType::MimageBuffer);
    stream.readRawData((char*)space.GetMimageBuffer(), sizeof(MimageData)*space.GetSpaceSize());

    _view->ClearObjects();
    _view->CreateVoxelObject(space.GetSpaceSize());

    file.close();

    _xSpaceLimiter->setRange(metadata.startPoint.x +
                             metadata.pointSize.x,
                             metadata.pointSize.x *
                             metadata.spaceUnit.x);
    _xSpaceLimiter->setValue(metadata.startPoint.x +
                             metadata.pointSize.x);

    _ySpaceLimiter->setRange(metadata.startPoint.y +
                             metadata.pointSize.y,
                             metadata.pointSize.y *
                             metadata.spaceUnit.y);
    _ySpaceLimiter->setValue(metadata.startPoint.y +
                             metadata.pointSize.y);

    _zSpaceLimiter->setRange(metadata.startPoint.z +
                             metadata.pointSize.z,
                             metadata.pointSize.z *
                             metadata.spaceUnit.z);
    _zSpaceLimiter->setValue(metadata.startPoint.z +
                             metadata.pointSize.z);

    connect(_xSpaceLimiter, SIGNAL(valueChanged(double)),
            this, SLOT(XSpaceLimiterChanged(double)));
    connect(_ySpaceLimiter, SIGNAL(valueChanged(double)),
            this, SLOT(YSpaceLimiterChanged(double)));
    connect(_zSpaceLimiter, SIGNAL(valueChanged(double)),
            this, SLOT(ZSpaceLimiterChanged(double)));
    UpdateMimageView();
}

void ViewerScreen::OpenModel(const QString &filePath)
{
    disconnect(_xSpaceLimiter, SIGNAL(valueChanged(double)),
               this, SLOT(XSpaceLimiterChanged(double)));
    disconnect(_ySpaceLimiter, SIGNAL(valueChanged(double)),
               this, SLOT(YSpaceLimiterChanged(double)));
    disconnect(_zSpaceLimiter, SIGNAL(valueChanged(double)),
               this, SLOT(ZSpaceLimiterChanged(double)));
    _mode = Mode::Model;
    _lowMimageLimiter->setVisible(false);
    _highMimageLimiter->setVisible(false);

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
        return;
    SpaceManager& space = SpaceManager::Self();
    QDataStream stream(&file);

    ModelMetadata metadata;
    stream.readRawData((char*)&metadata, sizeof(ModelMetadata));
    space.SetMetadata(metadata);
    space.InitFromMetadata();

    space.ActivateBuffer(SpaceManager::BufferType::ZoneBuffer);
    space.ResetBufferSize(metadata.zeroCount);

    _view->ClearObjects();
    _view->CreateVoxelObject(metadata.zeroCount);

    float voxSize = metadata.pointSize.x;
    Vector3f point;
    char value;
    Color color = ISpaceCalculator::GetModelColor();
    int spaceSize = space.GetSpaceSize();
    int zeroCounter = 0;
    for(int i = 0; i < spaceSize; ++i)
    {
        stream.readRawData((char*)&value, sizeof(char));
        if(value == 0)
        {
            space.GetZoneBuffer()[zeroCounter] = i;
            ++zeroCounter;
            point = space.GetPointCoords(i);

            _view->AddVoxelObject(point.x, point.y, point.z,
                                  color.red, color.green,
                                  color.blue, color.alpha);
        }
    }
    _view->Flush();
    file.close();

    _xSpaceLimiter->setRange(metadata.startPoint.x +
                             metadata.pointSize.x,
                             metadata.pointSize.x *
                             metadata.spaceUnit.x);
    _xSpaceLimiter->setValue(metadata.startPoint.x +
                             metadata.pointSize.x);

    _ySpaceLimiter->setRange(metadata.startPoint.y +
                             metadata.pointSize.y,
                             metadata.pointSize.y *
                             metadata.spaceUnit.y);
    _ySpaceLimiter->setValue(metadata.startPoint.y +
                             metadata.pointSize.y);

    _zSpaceLimiter->setRange(metadata.startPoint.z +
                             metadata.pointSize.z,
                             metadata.pointSize.z *
                             metadata.spaceUnit.z);
    _zSpaceLimiter->setValue(metadata.startPoint.z +
                             metadata.pointSize.z);
    connect(_xSpaceLimiter, SIGNAL(valueChanged(double)),
            this, SLOT(XSpaceLimiterChanged(double)));
    connect(_ySpaceLimiter, SIGNAL(valueChanged(double)),
            this, SLOT(YSpaceLimiterChanged(double)));
    connect(_zSpaceLimiter, SIGNAL(valueChanged(double)),
            this, SLOT(ZSpaceLimiterChanged(double)));
}

void ViewerScreen::LowMimageLimiterChanged(double value)
{
    _highMimageLimiter->setMinimum(value+0.05);
    UpdateMimageView();
}

void ViewerScreen::HighMimageLimiterChanged(double value)
{
    _lowMimageLimiter->setMaximum(value-0.05);
    UpdateMimageView();
}

void ViewerScreen::XSpaceLimiterChanged(double value)
{
    if(_mode == Mode::Model)
        UpdateZoneView();
    else
        UpdateMimageView();
}

void ViewerScreen::YSpaceLimiterChanged(double value)
{
    if(_mode == Mode::Model)
        UpdateZoneView();
    else
        UpdateMimageView();
}

void ViewerScreen::ZSpaceLimiterChanged(double value)
{
    if(_mode == Mode::Model)
        UpdateZoneView();
    else
        UpdateMimageView();
}

void ViewerScreen::UpdateMimageView()
{

    _view->ClearObjects(true);
    SpaceManager& space = SpaceManager::Self();
    float voxSize = space.GetMetadata().pointSize.x;
    Vector3f point;
    Color color;
    int spaceSize = space.GetSpaceSize();
    MimageData mValue;
    double limitValue;
    for(int i = 0; i < spaceSize; ++i)
    {
        mValue = space.GetMimage(i);
        limitValue = mValue.Cx;
        if(limitValue <= _highMimageLimiter->value() &&
                limitValue >= _lowMimageLimiter->value())
        {
            point = space.GetPointCoords(i);
            if(point.x >= _xSpaceLimiter->value() &&
                    point.y >= _ySpaceLimiter->value() &&
                    point.z >= _zSpaceLimiter->value())
            {
                color = ISpaceCalculator::GetMImageColor(limitValue);

                _view->AddVoxelObject(point.x, point.y, point.z,
                                      color.red, color.green,
                                      color.blue, color.alpha);
            }
        }
    }
    _view->Flush();
}

void ViewerScreen::UpdateZoneView()
{
    _view->ClearObjects(true);
    SpaceManager& space = SpaceManager::Self();
    float voxSize = space.GetMetadata().pointSize.x;
    Vector3f point;
    int rawId;
    Color color = ISpaceCalculator::GetModelColor();
    int bufferSize = space.GetBufferSize();
    for(int i = 0; i < bufferSize; ++i)
    {
        rawId = space.GetZone(i);
        point = space.GetPointCoords(rawId);
        if(point.x >= _xSpaceLimiter->value() &&
                point.y >= _ySpaceLimiter->value() &&
                point.z >= _zSpaceLimiter->value())
        {
            _view->AddVoxelObject(point.x, point.y, point.z,
                                  color.red, color.green,
                                  color.blue, color.alpha);
        }
    }
    _view->Flush();
}

