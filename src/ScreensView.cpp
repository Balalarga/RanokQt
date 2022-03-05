#include "ScreensView.h"
#include "Screens/Screens.h"

#include <QMessageBox>

ScreensView::ScreensView(QWidget *parent):
    QTabWidget(parent)
{
    auto viewerScreen = new ViewerScreen(this);
    auto raymarchingScreen = new RayMarchingScreen(this);
    _tabs.push_back(raymarchingScreen);
    _tabs.push_back(viewerScreen);
    addTab(raymarchingScreen, QIcon("assets/images/Code.svg"), "Моделирование");
    addTab(viewerScreen, QIcon("assets/images/Cube1.svg"), "Просмотр");
    setTabPosition(QTabWidget::West);
    setIconSize({30, 30});
    setTabsClosable(false);

    connect(this, &QTabWidget::currentChanged, this, &ScreensView::TabChanged);
}

bool ScreensView::ReadyToClose()
{
    if (auto view = dynamic_cast<RayMarchingScreen*>(_tabs[currentIndex()]))
    {
        if (!view->NeedSave())
            return true;

        QMessageBox::StandardButton button = QMessageBox::question(this,
                                                                   "Save all tabs",
                                                                   "Do you want to save all text files?",
                                                                   QMessageBox::StandardButton::Yes |
                                                                   QMessageBox::StandardButton::No |
                                                                   QMessageBox::StandardButton::Cancel);
        if (button == QMessageBox::StandardButton::Yes)
            view->SaveAll();
        else if(button == QMessageBox::StandardButton::Cancel)
            return false;
        return true;
    }
    return true;
}

void ScreensView::TabChanged(int id)
{
    for(int i = 0; i < count(); ++i)
        if(i != id)
            _tabs[i]->Cleanup();
}
