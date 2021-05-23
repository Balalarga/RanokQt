#include "matrixcalculator.h"
#include <vector>

using namespace std;

MatrixCalculator::MatrixCalculator(Vector3i step):
    m_step(step)
{

}

const std::deque<VoxelData> &MatrixCalculator::Calculate(Program &program, Zone zone, std::function<void(VoxelData&)> iterFunc)
{
    auto args = program.GetArgs();

    if(args.size() == 1)
        return matrix1(program, zone, iterFunc);
    else if(args.size() == 2)
        return matrix2(program, zone, iterFunc);
    else
        return matrix3(program, zone, iterFunc);
}

const std::deque<VoxelData> &MatrixCalculator::matrix1(Program &program, Zone zone, std::function<void (VoxelData &)> iterFunc)
{
    auto args = program.GetArgs();
    Vector3 size{
        (args[0].limits.second-args[0].limits.first)/m_step.x(),
        (args[0].limits.second-args[0].limits.first)/m_step.x(),
        (args[0].limits.second-args[0].limits.first)/m_step.x(),
    };
    Vector3 halfSize{
        size.x()/2.,
        size.x()/2.,
        size.x()/2.,
    };

    double x = args[0].limits.first + halfSize.x();
    while(x < args[0].limits.second)
    {
        vector<pair<Vector3, double>> values{
            {{ x+halfSize.x(), 0, 0 }, 0},
            {{ x-halfSize.x(), 0, 0 }, 0}
        };
        for(int i = 0; i < 2; i++)
            values[i].second = program.Compute(values[i].first);
        ZoneFlags flags = GetZoneFlags(values);
        if(CheckZone(zone, flags))
        {
            m_results->push_back(VoxelData({x, 0, 0}, halfSize, GetVoxelColor(), values, 1));
            if(iterFunc)
                iterFunc(m_results->back());
        }
        x+= size.x();
    }
    return *m_results;
}

const std::deque<VoxelData> &MatrixCalculator::matrix2(Program &program, Zone zone, std::function<void (VoxelData &)> iterFunc)
{
    auto args = program.GetArgs();
    Vector3 size{
        (args[0].limits.second-args[0].limits.first)/m_step.x(),
        (args[1].limits.second-args[1].limits.first)/m_step.y(),
        (args[1].limits.second-args[1].limits.first)/m_step.y(),
    };
    Vector3 halfSize{
        size.x()/2.,
        size.y()/2.,
        size.y()/2.,
    };

    double x = args[0].limits.first + halfSize.x();
    while(x < args[0].limits.second)
    {
        double y = args[1].limits.first + halfSize.y();
        while(y < args[1].limits.second)
        {
            vector<pair<Vector3, double>> values{
                {{ x+halfSize.x(), y+halfSize.y(), 0 }, 0},
                {{ x+halfSize.x(), y-halfSize.y(), 0 }, 0},
                {{ x-halfSize.x(), y+halfSize.y(), 0 }, 0},
                {{ x-halfSize.x(), y-halfSize.y(), 0 }, 0}
            };
            for(int i = 0; i < 4; i++)
                values[i].second = program.Compute(values[i].first);
            ZoneFlags flags = GetZoneFlags(values);
            if(CheckZone(zone, flags))
            {
                m_results->push_back(VoxelData({x, y, 0}, halfSize, GetVoxelColor(), values, 2));
                if(iterFunc)
                    iterFunc(m_results->back());
            }
            y+= size.y();
        }
        x+= size.x();
    }
    return *m_results;
}

const std::deque<VoxelData> &MatrixCalculator::matrix3(Program &program, Zone zone, std::function<void (VoxelData &)> iterFunc)
{
    auto args = program.GetArgs();
    Vector3 size{
        (args[0].limits.second-args[0].limits.first)/m_step.x(),
                (args[1].limits.second-args[1].limits.first)/m_step.y(),
                (args[2].limits.second-args[2].limits.first)/m_step.z(),
    };
    Vector3 halfSize{
        size.x()/2.,
                size.y()/2.,
                size.z()/2.,
    };

    double x = args[0].limits.first + halfSize.x();
    while(x < args[0].limits.second)
    {
        double y = args[1].limits.first + halfSize.y();
        while(y < args[1].limits.second)
        {
            double z = args[2].limits.first + halfSize.z();
            while(z < args[2].limits.second)
            {
                vector<pair<Vector3, double>> values{
                    {{ x+halfSize.x(), y+halfSize.y(), z+halfSize.z() }, 0},
                    {{ x+halfSize.x(), y+halfSize.y(), z-halfSize.z() }, 0},
                    {{ x+halfSize.x(), y-halfSize.y(), z+halfSize.z() }, 0},
                    {{ x+halfSize.x(), y-halfSize.y(), z-halfSize.z() }, 0},
                    {{ x-halfSize.x(), y+halfSize.y(), z+halfSize.z() }, 0},
                    {{ x-halfSize.x(), y+halfSize.y(), z-halfSize.z() }, 0},
                    {{ x-halfSize.x(), y-halfSize.y(), z+halfSize.z() }, 0},
                    {{ x-halfSize.x(), y-halfSize.y(), z-halfSize.z() }, 0}
                };
                for(int i = 0; i < 8; i++)
                    values[i].second = program.Compute(values[i].first);
                ZoneFlags flags = GetZoneFlags(values);
                if(CheckZone(zone, flags))
                {
                    m_results->push_back(VoxelData({x, y, z}, halfSize, GetVoxelColor(), values));
                    if(iterFunc)
                        iterFunc(m_results->back());
                }
                z+= size.z();
            }
            y+= size.y();
        }
        x+= size.x();
    }
    return *m_results;
}
