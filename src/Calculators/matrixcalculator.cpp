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
                glm::vec3 currentCoords[] = {
                    { x+size.x, y+size.y, z+size.z },
                    { x+size.x, y+size.y, z-size.z },
                    { x+size.x, y-size.y, z+size.z },
                    { x+size.x, y-size.y, z-size.z },
                    { x-size.x, y+size.y, z+size.z },
                    { x-size.x, y+size.y, z-size.z },
                    { x-size.x, y-size.y, z+size.z },
                    { x-size.x, y-size.y, z-size.z }
                };
                double currentValues[8];
                ZoneFlags flags;
                for(int i = 0; i < 8; i++)
                {
//                    auto it = storage.find({currentCoords[i]});
//                    if(it != storage.end())
//                    {
//                        currentValues[i] = it->second;
//                    }
//                    else
//                    {
                        currentValues[i] = program.Compute(currentCoords[i]);
//                        storage[{currentCoords[i]}] = currentValues[i];
//                    }

                    if(currentValues[i] > 0)
                        flags.plus = true;
                    if(currentValues[i] < 0)
                        flags.minus = true;
                    if(currentValues[i] == 0)
                        flags.zero = true;
                }
                if(CheckZone(zone, flags))
                {
                    m_results->push_back(VoxelData({x, y, z}, size, {1, 1, 1, 0.2}));
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
