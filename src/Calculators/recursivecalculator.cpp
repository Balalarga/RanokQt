#include "recursivecalculator.h"

#include <iostream>
using namespace std;

RecursiveCalculator::RecursiveCalculator(int depth):
    m_depth(depth)
{

}

const std::deque<VoxelData> &RecursiveCalculator::Calculate(Program &program, Zone zone)
{
    m_results->clear();
    auto args = program.GetArgs();
    if(args.size() != 3)
    {
        cout<<"Arguments count error: "<<args.size()<<endl;
        return *m_results;
    }
    glm::vec3 voxelSize{
        (args[0].limits.second-args[0].limits.first)/2.,
        (args[1].limits.second-args[1].limits.first)/2.,
        (args[2].limits.second-args[2].limits.first)/2.,
    };
    glm::vec3 center = {args[0].limits.first+voxelSize.x,
                        args[1].limits.first+voxelSize.y,
                        args[2].limits.first+voxelSize.z};

    m_program = &program;
    m_zone = zone;

    recursionFunc(center, voxelSize, m_depth);

    return *m_results;
}

void RecursiveCalculator::recursionFunc(glm::vec3 coords, glm::vec3 size, int step)
{
    if(step == 0)
    {
        vector<pair<glm::vec3, double>> values{
            {{ coords.x+size.x, coords.y+size.y, coords.z+size.z }, 0},
            {{ coords.x+size.x, coords.y+size.y, coords.z-size.z }, 0},
            {{ coords.x+size.x, coords.y-size.y, coords.z+size.z }, 0},
            {{ coords.x+size.x, coords.y-size.y, coords.z-size.z }, 0},
            {{ coords.x-size.x, coords.y+size.y, coords.z+size.z }, 0},
            {{ coords.x-size.x, coords.y+size.y, coords.z-size.z }, 0},
            {{ coords.x-size.x, coords.y-size.y, coords.z+size.z }, 0},
            {{ coords.x-size.x, coords.y-size.y, coords.z-size.z }, 0}
        };
        for(int i = 0; i < 8; i++)
            values[i].second = m_program->Compute(values[i].first);
        ZoneFlags flags = GetZoneFlags(values);
        if(CheckZone(m_zone, flags))
        {
            m_results->push_back(VoxelData(coords, size, {1, 1, 1, 0.2}, values));
            if(m_iterFunc)
                m_iterFunc(m_results->back());
        }
    }
    else
    {
        glm::vec3 newSize = {size.x/2., size.y/2., size.z/2.};
        glm::vec3 newCubes[] = {
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
            recursionFunc(newCubes[i], newSize, step-1);
        }
    }
}
