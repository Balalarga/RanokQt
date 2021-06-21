#include "MatrixCalculator.h"
#include <vector>

using namespace std;

MatrixCalculator::MatrixCalculator(Vector3i step, QObject *parent):
    ModelCalculator(parent),
    m_step(step)
{

}

void MatrixCalculator::Calculate()
{
    ClearResults();
    auto args = GetProgram()->GetArgs();

    if(args.size() == 1)
        Calculate1d();
    else if(args.size() == 2)
        Calculate2d();
    else
        Calculate3d();
}

void MatrixCalculator::Calculate1d()
{
    auto program = GetProgram();
    auto args = program->GetArgs();
    Vector3f size{
        static_cast<float>((args[0].limits.second-args[0].limits.first)/m_step.x),
                static_cast<float>((args[0].limits.second-args[0].limits.first)/m_step.x),
                static_cast<float>((args[0].limits.second-args[0].limits.first)/m_step.x),
    };
    Vector3f halfSize{
        size.x/2.f,
                size.x/2.f,
                size.x/2.f,
    };

    float x = args[0].limits.first + halfSize.x;
    while(x < args[0].limits.second)
    {
        vector<Vector3d> vertices{
            { x+halfSize.x, 0, 0 },
            { x-halfSize.x, 0, 0 }
        };
        VoxelValues values(vertices.size());
        for(size_t i = 0; i < vertices.size(); i++)
            values[i]= program->Compute(vertices[i]);
        if(CheckZone(values) == GetZone())
        {
            AddVoxel(VoxelData({x, 0, 0}, halfSize, GetVoxelColor()));
            AddVoxelValues(values);
            if(m_addVoxelFunc)
                m_addVoxelFunc(VoxelData({x, 0, 0}, halfSize, GetVoxelColor()));
        }
        x+= size.x;
    }
}

void MatrixCalculator::Calculate2d()
{
    auto program = GetProgram();
    auto args = program->GetArgs();
    Vector3f size{
        static_cast<float>((args[0].limits.second-args[0].limits.first)/m_step.x),
                static_cast<float>((args[1].limits.second-args[1].limits.first)/m_step.y),
                static_cast<float>((args[1].limits.second-args[1].limits.first)/m_step.y),
    };
    Vector3f halfSize{
        size.x/2.f,
                size.y/2.f,
                size.y/2.f,
    };

    float x = args[0].limits.first + halfSize.x;
    while(x < args[0].limits.second)
    {
        float y = args[1].limits.first + halfSize.y;
        while(y < args[1].limits.second)
        {
            vector<Vector3d> vertices
            {
                { x+halfSize.x, y+halfSize.y, 0 },
                { x+halfSize.x, y-halfSize.y, 0 },
                { x-halfSize.x, y+halfSize.y, 0 },
                { x-halfSize.x, y-halfSize.y, 0 },
            };
            VoxelValues values(vertices.size());
            for(size_t i = 0; i < vertices.size(); i++)
                values[i]= program->Compute(vertices[i]);
            if(CheckZone(values) == GetZone())
            {
                AddVoxel(VoxelData({x, y, 0}, halfSize, GetVoxelColor()));
                AddVoxelValues(values);
                if(m_addVoxelFunc)
                    m_addVoxelFunc(VoxelData({x, 0, 0}, halfSize, GetVoxelColor()));
            }
            y+= size.y;
        }
        x+= size.x;
    }
}

void MatrixCalculator::Calculate3d()
{
    auto program = GetProgram();
    auto args = program->GetArgs();
    Vector3f size{
        static_cast<float>((args[0].limits.second-args[0].limits.first)/m_step.x),
                static_cast<float>((args[1].limits.second-args[1].limits.first)/m_step.y),
                static_cast<float>((args[2].limits.second-args[2].limits.first)/m_step.z),
    };
    Vector3f halfSize{
        size.x/2.f,
                size.y/2.f,
                size.z/2.f,
    };

    float x = args[0].limits.first + halfSize.x;
    while(x < args[0].limits.second)
    {
        float y = args[1].limits.first + halfSize.y;
        while(y < args[1].limits.second)
        {
            float z = args[2].limits.first + halfSize.z;
            while(z < args[2].limits.second)
            {
                vector<Vector3d> vertices{
                    { x+halfSize.x, y+halfSize.y, z+halfSize.z },
                    { x+halfSize.x, y+halfSize.y, z-halfSize.z },
                    { x+halfSize.x, y-halfSize.y, z+halfSize.z },
                    { x+halfSize.x, y-halfSize.y, z-halfSize.z },
                    { x-halfSize.x, y+halfSize.y, z+halfSize.z },
                    { x-halfSize.x, y+halfSize.y, z-halfSize.z },
                    { x-halfSize.x, y-halfSize.y, z+halfSize.z },
                    { x-halfSize.x, y-halfSize.y, z-halfSize.z }
                };
                VoxelValues values(vertices.size());
                for(size_t i = 0; i < vertices.size(); i++)
                    values[i]= program->Compute(vertices[i]);
                if(CheckZone(values) == GetZone())
                {
                    AddVoxel(VoxelData({x, y, z}, halfSize, GetVoxelColor()));
                    AddVoxelValues(values);
                    if(m_addVoxelFunc)
                        m_addVoxelFunc(VoxelData({x, 0, 0}, halfSize, GetVoxelColor()));
                }
                z+= size.z;
            }
            y+= size.y;
        }
        x+= size.x;
    }
}
