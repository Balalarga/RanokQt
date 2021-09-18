#include "ScreensView.h"
#include "Screens/Screens.h"

ScreensView::ScreensView(QWidget *parent):
    QTabWidget(parent)
{
    addTab(new ModelingScreen(), QIcon("assets/images/Code.svg"), "");
    addTab(new ViewerScreen(), QIcon("assets/images/Cube1.svg"), "");
    setTabPosition(QTabWidget::West);
    setIconSize({30, 30});
    setTabsClosable(false);
}
