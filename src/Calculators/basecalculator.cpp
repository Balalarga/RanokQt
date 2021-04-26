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
    ofstream fileStream(file);
    if(!fileStream)
    {
        cout<<"Cannot open file "<<file<<endl;
        return;
    }
    for(auto i = m_results->begin(); i != m_results->end(); i++)
    {
        fileStream << i->center.x << endl;
        fileStream << i->center.y << endl;
        fileStream << i->center.z << endl;
        fileStream << i->size.x << endl;
        fileStream << i->size.y << endl;
        fileStream << i->size.z << endl;
        fileStream << i->color.r << endl;
        fileStream << i->color.g << endl;
        fileStream << i->color.b << endl;
        fileStream << i->color.a << endl;
        for(auto& v: i->values)
        {
            fileStream << v.first.x << endl;
            fileStream << v.first.y << endl;
            fileStream << v.first.z << endl;
            fileStream << v.second << endl;
        }
    }
    fileStream.close();
}

void BaseCalculator::LoadDataFromFile(std::string file)
{
    fstream fileStream(file);
    if(!fileStream)
    {
        cout<<"Cannot open file "<<file<<endl;
        return;
    }
    VoxelData data;
    while(!fileStream.eof())
    {
        fileStream >> data.center.x >> data.center.y >> data.center.z;
        fileStream >> data.size.x >> data.size.y >> data.size.z;
        fileStream >> data.color.r >> data.color.g >> data.color.b >> data.color.a;
        for(auto& v: data.values)
        {
            fileStream >> v.first.x >> v.first.y >> v.first.z;
            fileStream >> v.second;
        }
        m_results->push_back(data);
    }
    fileStream.close();
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
