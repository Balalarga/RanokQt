#ifndef OPENGLOBJECT_H
#define OPENGLOBJECT_H

#include <QColor>
#include <QObject>
#include <QtOpenGL>

#include "Utils.h"

class OpenglObject : public QObject
{
    Q_OBJECT
public:
    explicit OpenglObject(QColor color, QObject *parent = nullptr);
    virtual ~OpenglObject();

    virtual void Render() = 0;

    inline QColor GetColor(){ return m_color; }
    inline void SetColor(QColor color){ m_color = color; }

    inline void SetVisible(bool visible){ _visible = visible; }
    inline bool IsVisible(){ return _visible; }

private:
    QColor m_color;
    bool _visible;
};

#endif // OPENGLOBJECT_H
