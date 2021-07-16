#include "VoxelObject.h"

bool VoxelObject::Create(int count, QOpenGLShaderProgram* shader)
{
    bufferSize = sizeof(float)*count;
    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(nullptr, bufferSize*sizeof(float));
    shader->enableAttributeArray(0);
    shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    vao.release();
    vbo.release();
    isCreated = vbo.isCreated() && vao.isCreated();
    return isCreated;
}

void VoxelObject::AddData(float x, float y, float z)
{
    buffer.push_back(x);
    buffer.push_back(y);
    buffer.push_back(z);
    if(buffer.size() >= flushCount)
        Flush();
}

void VoxelObject::Flush()
{
    vbo.bind();
    vbo.write(bufferFill*sizeof(float), buffer.data(), buffer.size());
    bufferFill += buffer.size();
    buffer.clear();
}

void VoxelObject::Destroy()
{
    vao.destroy();
    vbo.destroy();
    isCreated = false;
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
