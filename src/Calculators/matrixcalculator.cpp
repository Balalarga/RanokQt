#include "matrixcalculator.h"
#include <queue>

using namespace std;

MatrixCalculator::MatrixCalculator(const std::vector<int> &step):
    m_step(step)
{

}

const std::deque<VoxelData> &MatrixCalculator::Calculate(Program &program, Zone zone)
{
    auto args = program.GetArgs();
    double size = (args[0].limits.second - args[0].limits.first)/m_step[0];
    for(int i = 1; i < args.size(); i++)
    {
        double newSize = (args[i].limits.second - args[i].limits.first)/m_step[i];
        if(size > newSize)
            size = newSize;
    }
    double halfSize = size/2.;
    double x = args[0].limits.first + halfSize;
    int counter = 0;
    while(x <= args[0].limits.second)
    {
        double y = args[1].limits.first + halfSize;
        while(y <= args[1].limits.second)
        {
            double z = args[2].limits.first + halfSize;
            while(z <= args[2].limits.second)
            {
                vector<pair<glm::vec3, double>> values{
                    {{ x+halfSize, y+halfSize, z+halfSize }, 0},
                    {{ x+halfSize, y+halfSize, z-halfSize }, 0},
                    {{ x+halfSize, y-halfSize, z+halfSize }, 0},
                    {{ x+halfSize, y-halfSize, z-halfSize }, 0},
                    {{ x-halfSize, y+halfSize, z+halfSize }, 0},
                    {{ x-halfSize, y+halfSize, z-halfSize }, 0},
                    {{ x-halfSize, y-halfSize, z+halfSize }, 0},
                    {{ x-halfSize, y-halfSize, z-halfSize }, 0}
                };
                ZoneFlags flags;
                for(int i = 0; i < 8; i++)
                {
                    values[i].second = program.Compute(values[i].first);
                    if(values[i].second > 0)
                        flags.plus = true;
                    if(values[i].second < 0)
                        flags.minus = true;
                    if(values[i].second == 0)
                        flags.zero = true;
                }
                if(CheckZone(zone, flags))
                {
                    m_results->push_back(VoxelData({x, y, z}, size, {1, 1, 1, 0.2}, values));
                    if(m_iterFunc)
                        m_iterFunc(m_results->back());
                    counter++;
                }
                z+= size;
            }
            y+= size;
        }
        x+= size;
    }
    return *m_results;
}
