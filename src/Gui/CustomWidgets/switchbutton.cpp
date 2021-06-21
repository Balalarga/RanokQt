#include "switchbutton.h"

#include <QPainter>

SwitchButton::SwitchButton(QWidget *parent) :
    QAbstractButton(parent)
{
    setCheckable(true);
}

void SwitchButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::gray);
    QRect switchRect;
    QColor switchColor;
    QString text;
    int halfWidth = width() / 2;
    if (isChecked()) {
      switchRect = QRect(halfWidth, 0, halfWidth, height());
      switchColor = QColor(Qt::cyan);
      text = "On";
    } else {
      switchRect = QRect(0, 0, halfWidth, height());
      switchColor = QColor(Qt::darkGray);
      text = "Off";
    }
    painter.fillRect(switchRect, switchColor);
    painter.drawText(switchRect, Qt::AlignCenter, text);
}
