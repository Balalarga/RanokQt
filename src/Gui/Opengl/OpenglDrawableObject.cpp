#include "OpenglDrawableObject.h"


OpenglDrawableObject::OpenglDrawableObject(const QVector<float> &vertices, const VaoLayout &vaoLayout, QObject *parent):
    QObject(parent),
    _primitive(GL_TRIANGLES)
{
    _dataSize = vertices.size();
    _vao.create();
    _vao.bind();

    _vbo.create();
    _vbo.bind();
    _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbo.allocate(vertices.data(), vertices.size()*sizeof(float));

    auto& layout = vaoLayout.GetLayoutItems();
    auto* shader = _shaderProgram->GetProgram();


    int offset = 0;
    for(int i = 0; i < layout.size(); ++i)
    {
        shader->enableAttributeArray(i);
        shader->setAttributeBuffer(i, layout[0].type, offset, layout[0].count, vaoLayout.GetStride());
        offset += layout[0].count * layout[0].size;
    }

    _vao.release();
    _vbo.release();
}

void OpenglDrawableObject::SetPrimitive(unsigned primitive)
{
    _primitive = primitive;
}

ShaderProgram &OpenglDrawableObject::GetShaderProgram()
{
    return *_shaderProgram;
}

void OpenglDrawableObject::Render()
{
    _shaderProgram->Bind();
    _vao.bind();

    glDrawArrays(_primitive, 0, _dataSize);

    _vao.release();
    _shaderProgram->Release();
}
