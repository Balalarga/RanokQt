#ifndef SHADERLAYOUT_H
#define SHADERLAYOUT_H

#include <QDebug>
#include <QVector>
#include "GL/gl.h"

struct VaoLayoutItem
{
    VaoLayoutItem();
    VaoLayoutItem(unsigned count, unsigned type);

    unsigned count;
    unsigned type;
    unsigned size;

    static unsigned GetTypeSize(unsigned type);
};

class VaoLayout
{
public:
    VaoLayout() = default;
    VaoLayout(const QVector<VaoLayoutItem>& items);

    void Add(unsigned count, unsigned type);

    inline const QVector<VaoLayoutItem>& GetLayoutItems() const
    {
        return _layoutItems;
    }

    inline unsigned GetStride() const
    {
        return _stride;
    }

    inline unsigned GetWidth() const
    {
        return _width;
    }

private:
    QVector<VaoLayoutItem> _layoutItems;
    unsigned _stride = 0;
    unsigned _width = 0;
};

#endif // SHADERLAYOUT_H
