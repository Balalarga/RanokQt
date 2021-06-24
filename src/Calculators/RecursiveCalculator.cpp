#include "RecursiveCalculator.h"

#include <iostream>
using namespace std;

RecursiveCalculator::RecursiveCalculator(int depth, QObject *parent):
    ModelCalculator(parent),
    m_depth(depth)
{
}

void RecursiveCalculator::Calculate()
{
    ClearResults();
    auto program = GetProgram();
    auto args = program->GetSymbolTable().GetAllArgs();

    Vector3f voxelSize;
    Vector3f coords;
    if(args.size() == 3)
    {
        voxelSize = {
            static_cast<float>((args[0]->limits.second-args[0]->limits.first)/2.f),
            static_cast<float>((args[1]->limits.second-args[1]->limits.first)/2.f),
            static_cast<float>((args[2]->limits.second-args[2]->limits.first)/2.f),};
        coords = {
            static_cast<float>(args[0]->limits.first+voxelSize.x),
            static_cast<float>(args[1]->limits.first+voxelSize.y),
            static_cast<float>(args[2]->limits.first+voxelSize.z)};
        Calculate3d(coords, voxelSize, m_depth);
    }
    else if(args.size() == 2)
    {
        voxelSize = {
            static_cast<float>((args[0]->limits.second-args[0]->limits.first)/2.f),
            static_cast<float>((args[1]->limits.second-args[1]->limits.first)/2.f),
            static_cast<float>((args[1]->limits.second-args[1]->limits.first)/2.f)};
        coords = {
            static_cast<float>(args[0]->limits.first+voxelSize.x),
            static_cast<float>(args[1]->limits.first+voxelSize.y),
            0};
        Calculate2d(coords, voxelSize, m_depth);
    }
    else if(args.size() == 1)
    {
        voxelSize = {
            static_cast<float>((args[0]->limits.second-args[0]->limits.first)/2.f),
            static_cast<float>((args[0]->limits.second-args[0]->limits.first)/2.f),
            static_cast<float>((args[0]->limits.second-args[0]->limits.first)/2.f)};
        coords = {
            static_cast<float>(args[0]->limits.first+voxelSize.x),
            0,
            0};
        Calculate1d(coords, voxelSize, m_depth);
    }
}

void RecursiveCalculator::Calculate1d(Vector3f coords, Vector3f size, int step)
{
    if(step == 0)
    {
        auto program = GetProgram();
        vector<Vector3d> vertices{
            { coords.x+size.x, 0, 0 },
            { coords.x-size.x, 0, 0 }
        };
        VoxelValues values(vertices.size());
        for(size_t i = 0; i < vertices.size(); i++)
            values[i]= program->Compute(vertices[i]);
        if(CheckZone(values) == GetZone())
        {
            AddVoxel(VoxelData(coords, size, GetVoxelColor()));
            AddVoxelValues(values);
            if(m_addVoxelFunc)
                m_addVoxelFunc(VoxelData(coords, size, GetVoxelColor()));
        }
    }
    else
    {
        Vector3f newSize = {size.x/2.f, size.x/2.f, size.x/2.f};
        Vector3f newCubes[] = {
            { coords.x+newSize.x, 0, 0 },
            { coords.x-newSize.x, 0, 0 }
        };
        for(int i = 0; i < 2; i++)
        {
            Calculate1d(newCubes[i], newSize, step-1);
        }
    }
}

void RecursiveCalculator::Calculate2d(Vector3f coords, Vector3f size, int step)
{
    if(step == 0)
    {
        auto program = GetProgram();
        vector<Vector3d> vertices{
            { coords.x+size.x, coords.y+size.y, 0 },
            { coords.x+size.x, coords.y-size.y, 0 },
            { coords.x-size.x, coords.y+size.y, 0 },
            { coords.x-size.x, coords.y-size.y, 0 }
        };
        VoxelValues values(vertices.size());
        for(size_t i = 0; i < vertices.size(); i++)
            values[i]= program->Compute(vertices[i]);
        if(CheckZone(values) == GetZone())
        {
            AddVoxel(VoxelData(coords, size, GetVoxelColor()));
            AddVoxelValues(values);
            if(m_addVoxelFunc)
                m_addVoxelFunc(VoxelData(coords, size, GetVoxelColor()));
        }
    }
    else
    {
        Vector3f newSize = {size.x/2.f, size.y/2.f, size.y/2.f };
        Vector3f newCubes[] = {
            { coords.x+newSize.x, coords.y+newSize.y, 0 },
            { coords.x+newSize.x, coords.y-newSize.y, 0 },
            { coords.x-newSize.x, coords.y+newSize.y, 0 },
            { coords.x-newSize.x, coords.y-newSize.y, 0 }
        };
        for(int i = 0; i < 4; i++)
        {
            Calculate2d(newCubes[i], newSize, step-1);
        }
    }
}

void RecursiveCalculator::Calculate3d(Vector3f coords, Vector3f size, int step)
{
    if(step == 0)
    {
        auto program = GetProgram();
        vector<Vector3d> vertices{
            { coords.x+size.x, coords.y+size.y, coords.z+size.z },
            { coords.x+size.x, coords.y+size.y, coords.z-size.z },
            { coords.x+size.x, coords.y-size.y, coords.z+size.z },
            { coords.x+size.x, coords.y-size.y, coords.z-size.z },
            { coords.x-size.x, coords.y+size.y, coords.z+size.z },
            { coords.x-size.x, coords.y+size.y, coords.z-size.z },
            { coords.x-size.x, coords.y-size.y, coords.z+size.z },
            { coords.x-size.x, coords.y-size.y, coords.z-size.z }
        };
        VoxelValues values(vertices.size());
        for(size_t i = 0; i < vertices.size(); i++)
            values[i]= program->Compute(vertices[i]);
        if(CheckZone(values) == GetZone())
        {
            AddVoxel(VoxelData(coords, size, GetVoxelColor()));
            AddVoxelValues(values);
            if(m_addVoxelFunc)
                m_addVoxelFunc(VoxelData(coords, size, GetVoxelColor()));
        }
    }
    else
    {
        Vector3f newSize = {size.x/2.f, size.y/2.f, size.z/2.f};
        Vector3f newCubes[] = {
            { coords.x+newSize.x, coords.y+newSize.y, coords.z+newSize.z },
            { coords.x+newSize.x, coords.y+newSize.y, coords.z-newSize.z },
            { coords.x+newSize.x, coords.y-newSize.y, coords.z+newSize.z },
            { coords.x+newSize.x, coords.y-newSize.y, coords.z-newSize.z },
            { coords.x-newSize.x, coords.y+newSize.y, coords.z+newSize.z },
            { coords.x-newSize.x, coords.y+newSize.y, coords.z-newSize.z },
            { coords.x-newSize.x, coords.y-newSize.y, coords.z+newSize.z },
            { coords.x-newSize.x, coords.y-newSize.y, coords.z-newSize.z }
        };
        for(int i = 0; i < 8; i++)
        {
            Calculate3d(newCubes[i], newSize, step-1);
        }
    }
}
