#ifndef VIEWERSCREEN_H
#define VIEWERSCREEN_H

#include <QWidget>
#include "Gui/Opengl/SceneView.h"

class ViewerScreen : public QWidget
{
    Q_OBJECT
public:
    explicit ViewerScreen(QWidget *parent = nullptr);


signals:


private slots:
    void OpenFile();


private:
    SceneView* _view;
};

#endif // VIEWERSCREEN_H
