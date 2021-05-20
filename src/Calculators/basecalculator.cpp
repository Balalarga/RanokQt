#include "basecalculator.h"

#include <fstream>
#include <sstream>

using namespace std;

inline bool operator<(const StorageItem& a, const StorageItem& b)
{
    return a.point.x < b.point.x && a.point.y < b.point.y && a.point.z < b.point.z;
}

BaseCalculator::BaseCalculator()
{
    m_results = new std::deque<VoxelData>;
}

BaseCalculator::~BaseCalculator()
{
    delete m_results;
}

const std::deque<VoxelData> &BaseCalculator::GetResults()
{
    return *m_results;
}

void BaseCalculator::SetIterationFunc(std::function<void (VoxelData &)> iterFunc)
{
    m_iterFunc = iterFunc;
}

void BaseCalculator::SaveDataToFile(std::string file)
{

}

void BaseCalculator::LoadDataFromFile(std::string file)
{

}

bool BaseCalculator::CheckZone(Zone zone, ZoneFlags flags) const
{
    switch (zone)
    {
    case Zone::Positive:
        return flags.plus;
    case Zone::Zero:
        return flags.zero || (flags.plus && flags.minus);
    case Zone::Negative:
        return flags.minus;
    }
}

ZoneFlags BaseCalculator::GetZoneFlags(const vector<pair<glm::vec3, double>>& values)
{
    ZoneFlags flags;
    for(int i = 0; i < 8; i++)
    {
        if(values[i].second > 0.)
            flags.plus = true;
        if(values[i].second < 0.)
            flags.minus = true;
        if(values[i].second == 0.)
            flags.zero = true;
    }
    return flags;
}
