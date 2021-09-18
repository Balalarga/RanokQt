#ifndef VIEWERSCREEN_H
#define VIEWERSCREEN_H

#include "ClearableWidget.h"
#include "Gui/Opengl/SceneView.h"

class ViewerScreen : public ClearableWidget
{
    Q_OBJECT
public:
    explicit ViewerScreen(QWidget *parent = nullptr);


public slots:
    void Cleanup() override;


private slots:
    void OpenFile();
    void OpenMimage(const QString& filePath);
    void OpenModel(const QString& filePath);

private:
    SceneView* _view;
};

#endif // VIEWERSCREEN_H
