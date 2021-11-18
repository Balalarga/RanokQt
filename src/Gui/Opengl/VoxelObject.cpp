#include "VoxelObject.h"

VoxelObject::VoxelObject(ShaderProgram *shaderProgram, const VaoLayout& vaoLayout, QObject *parent):
    OpenglDrawableObject(shaderProgram, vaoLayout, parent)
{
    SetPrimitive(GL_POINTS);
}

VoxelObject::~VoxelObject()
{
}

void VoxelObject::AddVoxel(float x, float y, float z, float r, float g, float b, float a)
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
    AddData(buffer);
    buffer.clear();
}

void VoxelObject::Destroy()
{
    buffer.clear();
    OpenglDrawableObject::Destroy();
}
