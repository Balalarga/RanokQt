#include "ViewerScreen.h"

#include <QFileDialog>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QDataStream>

#include "Space/SpaceManager.h"
#include "Space/Calculators/ISpaceCalculator.h"


ViewerScreen::ViewerScreen(QWidget *parent):
    ClearableWidget(parent),
    _view(new SceneView(this))
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);


    QMenuBar* menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("Файл");
    menuBar->addMenu(fileMenu);

    QAction* openAction = new QAction;
    openAction->setText("Открыть");
    connect(openAction, &QAction::triggered, this, &ViewerScreen::OpenFile);
    fileMenu->addAction(openAction);

    mainLayout->setMenuBar(menuBar);
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
        OpenModel(filePath);
    else if(filePath.endsWith(".ibin"))
        OpenMimage(filePath);
}

void ViewerScreen::OpenMimage(const QString &filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
        return;
    SpaceManager& space = SpaceManager::Self();
    QDataStream stream(&file);

    stream.readRawData((char*)&space.metadata, sizeof(SpaceManager::ModelMetadata));

    space.InitFromMetadata();

    _view->ClearObjects();
    _view->CreateVoxelObject(space.GetSpaceSize());

    cl_float3 point;
    MimageData data;
    Color color;
    int spaceSize = space.GetSpaceSize();
    for(int i = 0; i < spaceSize; ++i)
    {
        stream.readRawData((char*)&data, sizeof(MimageData));
        color = ISpaceCalculator::GetMImageColor(data.Cx);
        point = space.GetPointCoords(i);
        _view->AddObject(point.x, point.y, point.z,
                         color.red, color.green,
                         color.blue, color.alpha);
    }
    _view->Flush();

    file.close();
}

void ViewerScreen::OpenModel(const QString &filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
        return;
    SpaceManager& space = SpaceManager::Self();
    QDataStream stream(&file);

    stream.readRawData((char*)&space.metadata, sizeof(SpaceManager::ModelMetadata));

    space.InitFromMetadata();

    _view->ClearObjects();
    _view->CreateVoxelObject(space.metadata.zeroCount);

    cl_float3 point;
    int value;
    Color color = ISpaceCalculator::GetModelColor();
    int spaceSize = space.GetSpaceSize();
    for(int i = 0; i < spaceSize; ++i)
    {
        stream.readRawData((char*)&value, sizeof(int));
        if(value == 0)
        {
            point = space.GetPointCoords(i);
            _view->AddObject(point.x, point.y, point.z,
                             color.red, color.green,
                             color.blue, color.alpha);
        }
    }
    _view->Flush();

    file.close();
}
