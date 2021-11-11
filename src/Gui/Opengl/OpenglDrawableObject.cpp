#include "OpenglDrawableObject.h"


OpenglDrawableObject::OpenglDrawableObject(ShaderProgram* shaderProgram, const VaoLayout &vaoLayout, QObject *parent):
    QObject(parent),
    _shaderProgram(shaderProgram),
    _vaoLayout(vaoLayout)
{

}

OpenglDrawableObject::~OpenglDrawableObject()
{
    Destroy();
}

void OpenglDrawableObject::Create(const QVector<float>& vertices)
{
    _verticesCount = vertices.size()*sizeof(float)/_vaoLayout.GetStride();
    _verticesFilling = _verticesCount;
    _vao.create();
    _vao.bind();

    _vbo.create();
    _vbo.bind();
    _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbo.allocate(vertices.data(), vertices.size()*sizeof(float));

    auto& layout = _vaoLayout.GetLayoutItems();
    auto* shader = _shaderProgram->GetProgram();

    int offset = 0;
    for(int i = 0; i < layout.size(); ++i)
    {
        shader->enableAttributeArray(i);
        shader->setAttributeBuffer(i, layout[0].type, offset, layout[0].count, _vaoLayout.GetStride());
        offset += layout[0].size;
    }

    _vao.release();
    _vbo.release();
}

void OpenglDrawableObject::Destroy()
{
    _vao.release();
    _vbo.release();

    _vao.destroy();
    _vbo.destroy();
}

void OpenglDrawableObject::Create(unsigned verticesCount)
{
    _verticesCount = verticesCount;
    _verticesFilling = 0;
    _vao.create();
    _vao.bind();

    _vbo.create();
    _vbo.bind();
    _vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vbo.allocate(nullptr, verticesCount*_vaoLayout.GetStride()*sizeof(float));

    auto& layout = _vaoLayout.GetLayoutItems();
    auto* shader = _shaderProgram->GetProgram();

    int offset = 0;
    for(int i = 0; i < layout.size(); ++i)
    {
        shader->enableAttributeArray(i);
        shader->setAttributeBuffer(i, layout[0].type, offset, layout[0].count, _vaoLayout.GetStride());
        offset += layout[0].size;
    }

    _vao.release();
    _vbo.release();
}

void OpenglDrawableObject::AddData(const QVector<float>& vertices)
{
    if(_verticesFilling + vertices.size() <= _verticesCount)
    {
        _vbo.bind();
        _vbo.write(_verticesFilling * _vaoLayout.GetStride(), vertices.data(), vertices.size()*sizeof(float));
        _vbo.release();
        _verticesFilling += vertices.size();
    }
}

void OpenglDrawableObject::SetPrimitive(unsigned primitive)
{
    _primitive = primitive;
}

void OpenglDrawableObject::BindShader()
{
    _shaderProgram->Bind();
}

void OpenglDrawableObject::ReleaseShader()
{
    _shaderProgram->Release();
}

ShaderProgram* OpenglDrawableObject::GetShaderProgram()
{
    return _shaderProgram;
}

void OpenglDrawableObject::Render()
{
    _vao.bind();

    glDrawArrays(_primitive, 0, _verticesCount);

    _vao.release();
}

unsigned OpenglDrawableObject::GetLayoutSize() const
{
    return _vaoLayout.GetStride();
}

unsigned OpenglDrawableObject::GetFillingSize() const
{
    return _verticesFilling;
}

unsigned OpenglDrawableObject::GetSize() const
{
    return _verticesCount;
}
