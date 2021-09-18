#include "ScreensView.h"
#include "Screens/Screens.h"

ScreensView::ScreensView(QWidget *parent):
    QTabWidget(parent)
{
    auto modelingScreen = new ModelingScreen(this);
    auto viewerScreen = new ViewerScreen(this);
    _tabs.push_back(modelingScreen);
    _tabs.push_back(viewerScreen);
    addTab(modelingScreen, QIcon("assets/images/Code.svg"), "");
    addTab(viewerScreen, QIcon("assets/images/Cube1.svg"), "");
    setTabPosition(QTabWidget::West);
    setIconSize({30, 30});
    setTabsClosable(false);

    connect(this, &QTabWidget::currentChanged, this, &ScreensView::TabChanged);
}

void ScreensView::TabChanged(int id)
{
    for(int i = 0; i < count(); ++i)
        _tabs[i]->Cleanup();
}
