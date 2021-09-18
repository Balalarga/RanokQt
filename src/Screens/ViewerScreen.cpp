#include "ViewerScreen.h"

#include <QFileDialog>
#include <QMenuBar>
#include <QVBoxLayout>



ViewerScreen::ViewerScreen(QWidget *parent):
    QWidget(parent),
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

void ViewerScreen::OpenFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open file"), "",
                                                    tr("Binary file(*.mbin, *.ibin, *.bin)"));
    if(!fileName.isEmpty())
        return;
}
