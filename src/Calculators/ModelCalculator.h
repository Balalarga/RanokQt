#ifndef MODELCALCULATOR_H
#define MODELCALCULATOR_H

#include "ICalculator.h"


class ModelCalculator : public ICalculator
{
    Q_OBJECT
public:
    enum class Zone
    {
        Zero,
        Positive,
        Negative
    };
    using VoxelValues = std::vector<double>;


    explicit ModelCalculator(QObject *parent = nullptr);


    inline Zone GetZone(){ return m_zone; }
    inline void SetZone(const Zone& zone){ m_zone = zone; }


    void SaveDataToFile(std::string file) override;
    void LoadDataFromFile(std::string file) override;




protected:
    Zone CheckZone(const VoxelValues &values) const;


    inline void AddVoxelValues(VoxelValues values){ m_voxelValues.push_back(values); }
    inline std::vector<VoxelValues>& GetValues(){ return m_voxelValues; }


private:
    Zone m_zone;
    std::vector<VoxelValues> m_voxelValues;
};

#endif // MODELCALCULATOR_H
