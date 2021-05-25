#include "basecalculator.h"

#include <fstream>
#include <sstream>

using namespace std;

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

void BaseCalculator::SaveDataToFile(std::string file)
{

}

void BaseCalculator::LoadDataFromFile(std::string file)
{

}

void BaseCalculator::SetVoxelColor(sf::Color color)
{
    baseColor = color;
}

sf::Color BaseCalculator::GetVoxelColor()
{
    return baseColor;
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

ZoneFlags BaseCalculator::GetZoneFlags(const vector<pair<sf::Vector3<double>, double>>& values)
{
    ZoneFlags flags;
    for(int i = 0; i < values.size(); i++)
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
