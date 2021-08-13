#include "VoxelObject.h"

bool VoxelObject::Create(int count, QOpenGLShaderProgram* shader)
{
    vertexCount = count;
    bufferSize = sizeof(float)*count*7;
    vao.create();
    vao.bind();

    vbo.create();
    vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vbo.bind();
    vbo.allocate(nullptr, bufferSize);
    shader->enableAttributeArray(0);
    shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 7*sizeof(float));
    shader->enableAttributeArray(1);
    shader->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(float), 4, 7*sizeof(float));

    vao.release();
    vbo.release();
    isCreated = vbo.isCreated() && vao.isCreated();
    return isCreated;
}

void VoxelObject::AddData(float x, float y, float z, float r, float g, float b, float a)
{
    buffer.push_back(x);
    buffer.push_back(y);
    buffer.push_back(z);
    buffer.push_back(r);
    buffer.push_back(g);
    buffer.push_back(b);
    buffer.push_back(a);
    if(buffer.size() >= flushCount)
        Flush();
}

void VoxelObject::Flush()
{
    vbo.bind();
    vbo.write(bufferFill*7*sizeof(float), buffer.data(), buffer.size()*sizeof(float));
    vbo.release();

    bufferFill += buffer.size()/7;
    buffer.clear();
}

void VoxelObject::Destroy()
{
    vao.destroy();
    vbo.destroy();
    isCreated = false;
    bufferFill = 0;
    bufferSize = 0;
    buffer.clear();
}

void VoxelObject::Render()
{
    vao.bind();
    glDrawArrays(GL_POINTS, 0, bufferFill);
    vao.release();
}

bool VoxelObject::IsCreated()
{
    return isCreated;
}

void VoxelObject::Recreate(QOpenGLShaderProgram *shader)
{
    Destroy();
    Create(vertexCount, shader);
}
