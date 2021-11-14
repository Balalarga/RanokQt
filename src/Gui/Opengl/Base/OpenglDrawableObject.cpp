#include "OpenglDrawableObject.h"


OpenglDrawableObject::OpenglDrawableObject(ShaderProgram* shaderProgram, const VaoLayout &vaoLayout, QObject *parent):
    QObject(parent),
    _shaderProgram(shaderProgram),
    _vaoLayout(vaoLayout)
{

}

OpenglDrawableObject::~OpenglDrawableObject()
{
    OpenglDrawableObject::Destroy();
}

void OpenglDrawableObject::Create(const QVector<float>& vertices)
{
    _shaderProgram->Create();

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
    if(_vao.isCreated())
    {
        _vao.destroy();
        _vbo.destroy();
    }
}

void OpenglDrawableObject::Clear()
{
    Destroy();
    Create(_verticesCount);
}

bool OpenglDrawableObject::IsCreated() const
{
    return _vao.isCreated();
}

void OpenglDrawableObject::AddData(const QVector<float> &vertices)
{
    const float _fillingSize = _verticesFilling * _vaoLayout.GetStride();
    const float _maxSize = _verticesCount * _vaoLayout.GetStride();
    const float _writeSize = vertices.size() * sizeof(float);
    if(_fillingSize + _writeSize <= _maxSize)
    {
        _vbo.bind();
        _vbo.write(_fillingSize, vertices.data(), _writeSize);
        _vbo.release();
        _verticesFilling += vertices.size() / _vaoLayout.GetWidth();
    }
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
    _vbo.allocate(verticesCount * _vaoLayout.GetStride());

    auto& layout = _vaoLayout.GetLayoutItems();
    auto shader = _shaderProgram->GetProgram();

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
    if(_vao.isCreated())
    {
        _vao.bind();

        glDrawArrays(_primitive, 0, _verticesFilling);

        _vao.release();
    }
}

unsigned OpenglDrawableObject::GetLayoutSize() const
{
    return _vaoLayout.GetStride();
}

unsigned OpenglDrawableObject::GetFillingSize() const
{
    return _verticesFilling * _vaoLayout.GetStride();
}

unsigned OpenglDrawableObject::GetSize() const
{
    return _verticesCount * _vaoLayout.GetStride();
}
