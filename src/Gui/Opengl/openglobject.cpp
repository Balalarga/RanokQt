#include "openglobject.h"

OpenglObject::OpenglObject(QColor color, QObject *parent) :
    QObject(parent),
    m_color(color),
    _visible(true)
{

}

OpenglObject::~OpenglObject()
{

}
