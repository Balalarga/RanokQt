#ifndef SHADERLAYOUT_H
#define SHADERLAYOUT_H

#include <QDebug>
#include <QVector>
#include "GL/gl.h"

struct VaoLayoutItem
{
    unsigned count;
    unsigned type;
    unsigned size;
};

class VaoLayout
{
public:
    void Add(unsigned count, unsigned type)
    {
        unsigned size = 0;
        switch(type)
        {
        case GL_UNSIGNED_INT:
            size = sizeof(unsigned);
            break;
        case GL_INT:
            size = sizeof(int);
            break;
        case GL_FLOAT:
            size = sizeof(float);
            break;
        default:
            qDebug()<<"VaoLayout error: Unsupported data type";
        }

        _stride += size;
        _layoutItems.append({count, type, size});
    }

    inline const QVector<VaoLayoutItem>& GetLayoutItems() const
    {
        return _layoutItems;
    }

    unsigned GetStride() const
    {
        return _stride;
    }


private:
    QVector<VaoLayoutItem> _layoutItems;
    unsigned _stride = 0;
};

#endif // SHADERLAYOUT_H
