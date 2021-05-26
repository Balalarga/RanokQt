#include "RecursiveCalculator.h"

#include <iostream>
using namespace std;

RecursiveCalculator::RecursiveCalculator(int depth):
    m_depth(depth)
{
}

const std::deque<VoxelData> &RecursiveCalculator::Calculate(Program &program, Zone zone, std::function<void (VoxelData &)> iterFunc)
{
    m_results->clear();
    m_program = &program;
    m_zone = zone;

    auto args = program.GetArgs();
    stepsAmount = pow(pow(2, args.size()), m_depth);

    sf::Vector3<double> voxelSize;
    sf::Vector3<double> coords;
    if(args.size() == 3)
    {
        voxelSize = {
            (args[0].limits.second-args[0].limits.first)/2.,
            (args[1].limits.second-args[1].limits.first)/2.,
            (args[2].limits.second-args[2].limits.first)/2.,};
        coords = {
            args[0].limits.first+voxelSize.x,
            args[1].limits.first+voxelSize.y,
            args[2].limits.first+voxelSize.z};
        recursionFunc3(coords, voxelSize, m_depth, iterFunc);
    }
    else if(args.size() == 2)
    {
        voxelSize = {
            (args[0].limits.second-args[0].limits.first)/2.,
            (args[1].limits.second-args[1].limits.first)/2.,
            (args[1].limits.second-args[1].limits.first)/2.};
        coords = {
            args[0].limits.first+voxelSize.x,
            args[1].limits.first+voxelSize.y,
            0};
        recursionFunc2(coords, voxelSize, m_depth, iterFunc);
    }
    else if(args.size() == 1)
    {
        voxelSize = {
            (args[0].limits.second-args[0].limits.first)/2.,
            (args[0].limits.second-args[0].limits.first)/2.,
            (args[0].limits.second-args[0].limits.first)/2.};
        coords = {
            args[0].limits.first+voxelSize.x,
            0,
            0};
        recursionFunc1(coords, voxelSize, m_depth, iterFunc);
    }

    return *m_results;
}

void RecursiveCalculator::recursionFunc1(sf::Vector3<double> coords, sf::Vector3<double> size, int step, std::function<void (VoxelData &)> iterFunc)
{
    if(step == 0)
    {
        vector<pair<sf::Vector3<double>, double>> values{
            {{ coords.x+size.x, 0, 0 }, 0},
            {{ coords.x-size.x, 0, 0 }, 0}
        };
        for(int i = 0; i < 2; i++)
            values[i].second = m_program->Compute(values[i].first);
        Zone voxelZone = GetZone(values);
        m_results->push_back(VoxelData(coords, size, GetVoxelColor(), voxelZone, values, 1));
        if(iterFunc && m_zone == voxelZone)
        {
            iterFunc(m_results->back());
        }
    }
    else
    {
        sf::Vector3<double> newSize = {size.x/2., size.x/2., size.x/2.};
        sf::Vector3<double> newCubes[] = {
            { coords.x+newSize.x, 0, 0 },
            { coords.x-newSize.x, 0, 0 }
        };
        for(int i = 0; i < 2; i++)
        {
            recursionFunc1(newCubes[i], newSize, step-1, iterFunc);
        }
    }
}

void RecursiveCalculator::recursionFunc2(sf::Vector3<double> coords, sf::Vector3<double> size, int step, std::function<void (VoxelData &)> iterFunc)
{
    if(step == 0)
    {
        vector<pair<sf::Vector3<double>, double>> values{
            {{ coords.x+size.x, coords.y+size.y, 0 }, 0},
            {{ coords.x+size.x, coords.y-size.y, 0 }, 0},
            {{ coords.x-size.x, coords.y+size.y, 0 }, 0},
            {{ coords.x-size.x, coords.y-size.y, 0 }, 0}
        };
        for(int i = 0; i < 4; i++)
            values[i].second = m_program->Compute(values[i].first);
        Zone voxelZone = GetZone(values);
        m_results->push_back(VoxelData(coords, size, GetVoxelColor(), voxelZone, values, 2));
        if(iterFunc && m_zone == voxelZone)
        {
            iterFunc(m_results->back());
        }
    }
    else
    {
        sf::Vector3<double> newSize = {size.x/2., size.y/2., size.y/2. };
        sf::Vector3<double> newCubes[] = {
            { coords.x+newSize.x, coords.y+newSize.y, 0 },
            { coords.x+newSize.x, coords.y-newSize.y, 0 },
            { coords.x-newSize.x, coords.y+newSize.y, 0 },
            { coords.x-newSize.x, coords.y-newSize.y, 0 }
        };
        for(int i = 0; i < 4; i++)
        {
            recursionFunc2(newCubes[i], newSize, step-1, iterFunc);
        }
    }
}

void RecursiveCalculator::recursionFunc3(sf::Vector3<double> coords, sf::Vector3<double> size, int step, std::function<void (VoxelData &)> iterFunc)
{
    if(step == 0)
    {
        vector<pair<sf::Vector3<double>, double>> values{
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
        Zone voxelZone = GetZone(values);
        m_results->push_back(VoxelData(coords, size, GetVoxelColor(), voxelZone, values));
        if(iterFunc && m_zone == voxelZone)
        {
            iterFunc(m_results->back());
        }
    }
    else
    {
        sf::Vector3<double> newSize = {size.x/2., size.y/2., size.z/2.};
        sf::Vector3<double> newCubes[] = {
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
            recursionFunc3(newCubes[i], newSize, step-1, iterFunc);
        }
    }
}
