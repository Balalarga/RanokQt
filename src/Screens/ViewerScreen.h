#ifndef VIEWERSCREEN_H
#define VIEWERSCREEN_H

#include "ClearableWidget.h"
#include "Gui/Opengl/SceneView.h"

#include <QDoubleSpinBox>

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
    void LowMimageLimiterChanged(double value);
    void HighMimageLimiterChanged(double value);

    void XSpaceLimiterChanged(double value);
    void YSpaceLimiterChanged(double value);
    void ZSpaceLimiterChanged(double value);

    void UpdateMimageView();
    void UpdateZoneView();

private:
    SceneView* _view;
    QDoubleSpinBox* _lowMimageLimiter;
    QDoubleSpinBox* _highMimageLimiter;

    QDoubleSpinBox* _xSpaceLimiter;
    QDoubleSpinBox* _ySpaceLimiter;
    QDoubleSpinBox* _zSpaceLimiter;
};

#endif // VIEWERSCREEN_H
