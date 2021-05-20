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
    glm::vec3 size{
        (args[0].limits.second-args[0].limits.first)/m_step.x,
        (args[1].limits.second-args[1].limits.first)/m_step.y,
        (args[2].limits.second-args[2].limits.first)/m_step.z,
    };
    glm::vec3 halfSize{
        size.x/2.,
        size.y/2.,
        size.z/2.,
    };

    double x = args[0].limits.first + halfSize.x;

    int counter = 0;
    double xIters = (args[0].limits.second-args[0].limits.first)/size.x;

    while(x <= args[0].limits.second)
    {
        cout<<counter/xIters<<endl;
        double y = args[1].limits.first + halfSize.y;
        while(y <= args[1].limits.second)
        {
            double z = args[2].limits.first + halfSize.z;
            while(z <= args[2].limits.second)
            {
                vector<pair<glm::vec3, double>> values{
                    {{ x+halfSize.x, y+halfSize.y, z+halfSize.z }, 0},
                    {{ x+halfSize.x, y+halfSize.y, z-halfSize.z }, 0},
                    {{ x+halfSize.x, y-halfSize.y, z+halfSize.z }, 0},
                    {{ x+halfSize.x, y-halfSize.y, z-halfSize.z }, 0},
                    {{ x-halfSize.x, y+halfSize.y, z+halfSize.z }, 0},
                    {{ x-halfSize.x, y+halfSize.y, z-halfSize.z }, 0},
                    {{ x-halfSize.x, y-halfSize.y, z+halfSize.z }, 0},
                    {{ x-halfSize.x, y-halfSize.y, z-halfSize.z }, 0}
                };
                for(int i = 0; i < 8; i++)
                    values[i].second = program.Compute(values[i].first);
                ZoneFlags flags = GetZoneFlags(values);
                if(CheckZone(zone, flags))
                {
                    m_results->push_back(VoxelData({x, y, z}, size, {1, 1, 1, 0.2}, values));
                    if(m_iterFunc)
                        m_iterFunc(m_results->back());
                }
                z+= size.z;
            }
            y+= size.y;
        }
        x+= size.x;
        counter++;
    }
    return *m_results;
}
