#include "GridObject.h"

static const VaoLayout gridLayout = VaoLayout({
                                           VaoLayoutItem(3, GL_FLOAT),
                                           VaoLayoutItem(4, GL_FLOAT),
                                       });
static ShaderProgram gridShader({":/shaders/grid.vert", ":/shaders/grid.frag"});


GridObject::GridObject(QObject *parent):
    OpenglDrawableObject(&gridShader, gridLayout, parent)
{
    mainColor = QColor(255*0.5f, 255*0.5f,  255*0.7f, 255*0.5f);

    SetPrimitive(GL_LINES);

    gridShader.uniforms << "worldToView"
                        << "gridColor"
                        << "backColor";
}

void GridObject::SetMainColor(const QColor &color)
{
    mainColor = color;
    Destroy();
    Create();
}

void GridObject::Create()
{
    QVector<float> buffer;
    float x1 = -lineWidth*0.5;
    float x2 = lineWidth*0.5;
    for (int i = 0; i < lineWidth; ++i)
    {
        float d = lineWidth/(linesCount-1)*i-lineWidth*0.5;
        buffer.push_back(d);
        buffer.push_back(x1);
        buffer.push_back(0);
        buffer.push_back(mainColor.redF());
        buffer.push_back(mainColor.greenF());
        buffer.push_back(mainColor.blueF());
        buffer.push_back(mainColor.alphaF());

        buffer.push_back(d);
        buffer.push_back(x2);
        buffer.push_back(0);
        buffer.push_back(mainColor.redF());
        buffer.push_back(mainColor.greenF());
        buffer.push_back(mainColor.blueF());
        buffer.push_back(mainColor.alphaF());

        buffer.push_back(x1);
        buffer.push_back(d);
        buffer.push_back(0);
        buffer.push_back(mainColor.redF());
        buffer.push_back(mainColor.greenF());
        buffer.push_back(mainColor.blueF());
        buffer.push_back(mainColor.alphaF());

        buffer.push_back(x2);
        buffer.push_back(d);
        buffer.push_back(0);
        buffer.push_back(mainColor.redF());
        buffer.push_back(mainColor.greenF());
        buffer.push_back(mainColor.blueF());
        buffer.push_back(mainColor.alphaF());
    }

    gridShader.Create();
    OpenglDrawableObject::Create(buffer);
}
