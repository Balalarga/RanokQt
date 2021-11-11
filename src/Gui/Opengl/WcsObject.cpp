#include "WcsObject.h"


static const VaoLayout wcsLayout = VaoLayout({
                                           VaoLayoutItem(3, GL_FLOAT),
                                           VaoLayoutItem(4, GL_FLOAT),
                                       });
static ShaderProgram wcsShader({":/shaders/grid.vert", ":/shaders/grid.frag"});


WcsObject::WcsObject(QObject *parent):
    OpenglDrawableObject(&wcsShader, wcsLayout, parent)
{
    colors.push_back(QColor(255,   0,   0));
    colors.push_back(QColor(  0, 255,   0));
    colors.push_back(QColor(  0,   0, 255));

    SetPrimitive(GL_LINES);

    wcsShader.uniforms << "worldToView"
                       << "gridColor"
                       << "backColor";
}

void WcsObject::Create()
{
    QVector<float> buffer;
    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(colors[0].redF());
    buffer.push_back(colors[0].greenF());
    buffer.push_back(colors[0].blueF());
    buffer.push_back(colors[0].alphaF());

    buffer.push_back(1);
    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(colors[0].redF());
    buffer.push_back(colors[0].greenF());
    buffer.push_back(colors[0].blueF());
    buffer.push_back(colors[0].alphaF());

    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(colors[1].redF());
    buffer.push_back(colors[1].greenF());
    buffer.push_back(colors[1].blueF());
    buffer.push_back(colors[1].alphaF());

    buffer.push_back(0);
    buffer.push_back(1);
    buffer.push_back(0);
    buffer.push_back(colors[1].redF());
    buffer.push_back(colors[1].greenF());
    buffer.push_back(colors[1].blueF());
    buffer.push_back(colors[1].alphaF());

    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(colors[2].redF());
    buffer.push_back(colors[2].greenF());
    buffer.push_back(colors[2].blueF());
    buffer.push_back(colors[2].alphaF());

    buffer.push_back(0);
    buffer.push_back(0);
    buffer.push_back(1);
    buffer.push_back(colors[2].redF());
    buffer.push_back(colors[2].greenF());
    buffer.push_back(colors[2].blueF());
    buffer.push_back(colors[2].alphaF());

    wcsShader.Create();
    OpenglDrawableObject::Create(buffer);
}

void WcsObject::SetColors(const QVector<QColor>& colors)
{
    for(int i = 0; i < 3; i++)
        if(i < colors.size())
            this->colors[i] = colors[i];
    Destroy();
    Create();
}
