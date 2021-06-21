#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QAbstractButton>

class SwitchButton : public QAbstractButton
{
    Q_OBJECT
public:
    explicit SwitchButton(QWidget *parent = nullptr);


protected:
    void paintEvent(QPaintEvent*);

};

#endif // SWITCHBUTTON_H
