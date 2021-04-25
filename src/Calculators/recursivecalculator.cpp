#include "recursivecalculator.h"

#include <iostream>
using namespace std;

RecursiveCalculator::RecursiveCalculator(int depth):
    m_depth(depth)
{

}

std::deque<VoxelData> RecursiveCalculator::Calculate(Program& program, Zone zone)
{
    deque<VoxelData> data;
    auto args = program.GetArgs();
    if(args.size() != 3)
    {
        cout<<"Arguments count error: "<<args.size()<<endl;
        return data;
    }

    glm::vec3 voxelSize = {
        (args[0].limits.second - args[0].limits.first)/2.,
        (args[1].limits.second - args[1].limits.first)/2.,
        (args[2].limits.second - args[2].limits.first)/2.
    };
    glm::vec3 center = {
        args[0].limits.first + voxelSize.x,
        args[1].limits.first + voxelSize.y,
        args[2].limits.first + voxelSize.z
    };

    m_result = &data;
    m_program = &program;
    m_zone = zone;

    recursionFunc(center, voxelSize, m_depth);

    return data;
}

void RecursiveCalculator::recursionFunc(glm::vec3 coords, glm::vec3 size, int step)
{
    if(step == 1)
    {
        double currentValues[8];
        currentValues[0] = m_program->Compute({ coords.x+size.x, coords.y+size.y, coords.z+size.z });
        currentValues[1] = m_program->Compute({ coords.x+size.x, coords.y+size.y, coords.z-size.z });
        currentValues[2] = m_program->Compute({ coords.x+size.x, coords.y-size.y, coords.z+size.z });
        currentValues[3] = m_program->Compute({ coords.x+size.x, coords.y-size.y, coords.z-size.z });
        currentValues[4] = m_program->Compute({ coords.x-size.x, coords.y+size.y, coords.z+size.z });
        currentValues[5] = m_program->Compute({ coords.x-size.x, coords.y+size.y, coords.z-size.z });
        currentValues[6] = m_program->Compute({ coords.x-size.x, coords.y-size.y, coords.z+size.z });
        currentValues[7] = m_program->Compute({ coords.x-size.x, coords.y-size.y, coords.z-size.z });

        ZoneFlags flags;
        for(auto& i: currentValues)
        {
            if(i > 0)
                flags.plus = true;
            if(i < 0)
                flags.minus = true;
            if(i == 0)
                flags.zero = true;
        }
        if(CheckZone(m_zone, flags))
        {
            m_result->push_back(VoxelData(coords, size, {1, 1, 1, 0.1}));
        }
    }
    else
    {
        glm::vec3 newCubes[8];
        glm::vec3 newSize = { size.x / 2, size.y / 2, size.z / 2 };
        newCubes[0] = { coords.x+newSize.x, coords.y+newSize.y, coords.z+newSize.z };
        newCubes[1] = { coords.x+newSize.x, coords.y+newSize.y, coords.z-newSize.z };
        newCubes[2] = { coords.x+newSize.x, coords.y-newSize.y, coords.z+newSize.z };
        newCubes[3] = { coords.x+newSize.x, coords.y-newSize.y, coords.z-newSize.z };
        newCubes[4] = { coords.x-newSize.x, coords.y+newSize.y, coords.z+newSize.z };
        newCubes[5] = { coords.x-newSize.x, coords.y+newSize.y, coords.z-newSize.z };
        newCubes[6] = { coords.x-newSize.x, coords.y-newSize.y, coords.z+newSize.z };
        newCubes[7] = { coords.x-newSize.x, coords.y-newSize.y, coords.z-newSize.z };

        for(int i = 0; i < 8; i++)
            recursionFunc(newCubes[i], newSize, step-1);
    }
}
