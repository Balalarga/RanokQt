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
    double voxelSize = args[0].limits.second - args[0].limits.first;
    for(int i = 1; i < args.size(); i++)
    {
        double newSize = args[i].limits.second - args[i].limits.first;
        if(voxelSize > newSize)
            voxelSize = newSize;
    }
    voxelSize /= 2.;
    glm::vec3 center = {
        args[0].limits.first + voxelSize,
        args[1].limits.first + voxelSize,
        args[2].limits.first + voxelSize
    };

    m_program = &program;
    m_zone = zone;

    recursionFunc(center, voxelSize, m_depth);

    return *m_results;
}

void RecursiveCalculator::recursionFunc(glm::vec3 coords, double size, int step)
{
    if(step == 0)
    {
        vector<pair<glm::vec3, double>> values{
            {{ coords.x+size, coords.y+size, coords.z+size }, 0},
            {{ coords.x+size, coords.y+size, coords.z-size }, 0},
            {{ coords.x+size, coords.y-size, coords.z+size }, 0},
            {{ coords.x+size, coords.y-size, coords.z-size }, 0},
            {{ coords.x-size, coords.y+size, coords.z+size }, 0},
            {{ coords.x-size, coords.y+size, coords.z-size }, 0},
            {{ coords.x-size, coords.y-size, coords.z+size }, 0},
            {{ coords.x-size, coords.y-size, coords.z-size }, 0}
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
        double newSize = size/2.;
        glm::vec3 newCubes[] = {
            { coords.x+newSize, coords.y+newSize, coords.z+newSize },
            { coords.x+newSize, coords.y+newSize, coords.z-newSize },
            { coords.x+newSize, coords.y-newSize, coords.z+newSize },
            { coords.x+newSize, coords.y-newSize, coords.z-newSize },
            { coords.x-newSize, coords.y+newSize, coords.z+newSize },
            { coords.x-newSize, coords.y+newSize, coords.z-newSize },
            { coords.x-newSize, coords.y-newSize, coords.z+newSize },
            { coords.x-newSize, coords.y-newSize, coords.z-newSize }
        };
        for(int i = 0; i < 8; i++)
        {
            recursionFunc(newCubes[i], newSize, step-1);
        }
    }
}
