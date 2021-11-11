#include "VoxelObject.h"

static const VaoLayout voxelLayout = VaoLayout({
                                           VaoLayoutItem(3, GL_FLOAT),
                                           VaoLayoutItem(4, GL_FLOAT),
                                       });
static ShaderProgram voxelShader({":/shaders/voxel.vert",
                                  ":/shaders/voxel.frag",
                                  ":/shaders/voxel.geom"});


VoxelObject::VoxelObject(QObject *parent):
    OpenglDrawableObject(&voxelShader, voxelLayout, parent)
{
    SetPrimitive(GL_POINTS);
}

void VoxelObject::FullCreate(unsigned count)
{
    voxelShader.Create();
    OpenglDrawableObject::Create(count);
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
    bufferFill = 0;
    buffer.clear();
    OpenglDrawableObject::Destroy();
}
