#include "modelcalculator.h"

ModelCalculator::ModelCalculator(QObject *parent):
    ICalculator(parent),
    m_zone(Zone::Zero)
{

}

void ModelCalculator::SaveDataToFile(std::string file)
{

}

void ModelCalculator::LoadDataFromFile(std::string file)
{

}

ModelCalculator::Zone ModelCalculator::CheckZone(const VoxelValues &values) const
{
    bool plus = false;
    bool zero = false;
    bool minus = false;

    for(size_t i = 0; i < values.size(); i++)
    {
        if(values[i] == 0)
            zero = true;
        if(values[i] < 0)
            minus = true;
        if(values[i] > 0)
            plus = true;
    }

    if(zero || (plus && minus))
        return Zone::Zero;

    if(plus)
        return Zone::Positive;

    return Zone::Negative;
}
