#include "BaseCalculator.h"

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
    ofstream out(file);
    if(!out)
    {
        cout<<"Couldn't open file "<<file<<endl;
        return;
    }

    for(auto& i: *m_results)
    {
        out<<i.center.x<<' '<<i.center.y<<' '<<i.center.z<<' ';
        out<<i.size.x<<' '<<i.size.y<<' '<<i.size.z<<' ';
        out<<i.color.r<<' '<<i.color.g<<' '<<i.color.b<<' '<<i.color.a<<' ';
        out<<static_cast<int>(i.zone)<<'\n';
    }

    out.close();
}

void BaseCalculator::LoadDataFromFile(std::string file)
{
    ifstream in(file);
    if(!in)
    {
        cout<<"Couldn't open file "<<file<<endl;
        return;
    }
    VoxelData data;
    int zone;
    while(!in.eof())
    {
        in >> data.center.x >> data.center.y >> data.center.z;
        in >> data.size.x >> data.size.y >> data.size.z;
        in >> data.color.r >> data.color.g >> data.color.b >> data.color.a;
        in >> zone;
        data.zone = static_cast<Zone>(zone);
        m_results->push_back(data);
    }
    in.close();
}

void BaseCalculator::SetVoxelColor(sf::Color color)
{
    baseColor = color;
}

sf::Color BaseCalculator::GetVoxelColor()
{
    return baseColor;
}

Zone BaseCalculator::GetZone(const std::vector<std::pair<sf::Vector3<double>, double> > &values) const
{
    struct
    {
        bool plus = false;
        bool zero = false;
        bool minus = false;
    } flags;

    for(int i = 0; i < values.size(); i++)
    {
        if(values[i].second > 0.)
            flags.plus = true;
        if(values[i].second < 0.)
            flags.minus = true;
        if(values[i].second == 0.)
            flags.zero = true;
    }
    if(flags.zero || (flags.plus && flags.minus))
        return Zone::Zero;
    if(flags.plus)
        return Zone::Positive;
    return Zone::Negative;
}
