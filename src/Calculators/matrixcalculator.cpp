#include "matrixcalculator.h"
#include <queue>

using namespace std;

MatrixCalculator::MatrixCalculator(glm::vec3 step):
    m_step(step)
{

}

const std::deque<VoxelData> &MatrixCalculator::Calculate(Program &program, Zone zone)
{
    auto args = program.GetArgs();
    glm::vec3 size = {
        (args[0].limits.second - args[0].limits.first)/m_step.x,
        (args[1].limits.second - args[1].limits.first)/m_step.y,
        (args[2].limits.second - args[2].limits.first)/m_step.z
    };
    double x = args[0].limits.first + size.x;
    int counter = 0;
    while(x <= args[0].limits.second)
    {
        double y = args[1].limits.first + size.y;
        while(y <= args[1].limits.second)
        {
            double z = args[2].limits.first + size.z;
            while(z <= args[2].limits.second)
            {
                vector<pair<glm::vec3, double>> values{
                    {{ x+size.x, y+size.y, z+size.z }, 0},
                    {{ x+size.x, y+size.y, z-size.z }, 0},
                    {{ x+size.x, y-size.y, z+size.z }, 0},
                    {{ x+size.x, y-size.y, z-size.z }, 0},
                    {{ x-size.x, y+size.y, z+size.z }, 0},
                    {{ x-size.x, y+size.y, z-size.z }, 0},
                    {{ x-size.x, y-size.y, z+size.z }, 0},
                    {{ x-size.x, y-size.y, z-size.z }, 0}
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
                z+= size.z;
            }
            y+= size.y;
        }
//        x+= size.x;
    }
    return *m_results;
}
