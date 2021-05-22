#ifndef BASECALCULATOR_H
#define BASECALCULATOR_H

#include <deque>
#include <vector>
#include <functional>

#include "Vectors.h"
#include "Color.h"
#include "Language/program.h"

enum class Zone
{
    Zero,
    Positive,
    Negative
};

struct ZoneFlags
{
    bool plus = false;
    bool zero = false;
    bool minus = false;
};

struct VoxelData
{
    VoxelData(){};
    VoxelData(const Vector3& center, const Vector3& size,
              const Color& color, std::vector<std::pair<Vector3, double>>& values,
              int dementions = 3):
        center(center), size(size), color(color), values(values),
        dementions(dementions)
    {}
    Vector3 center;
    Vector3 size;
    Color color;
    std::vector<std::pair<Vector3, double>> values;
    int dementions;
};

class BaseCalculator
{
public:
    BaseCalculator();
    virtual ~BaseCalculator();
    virtual const std::deque<VoxelData>& Calculate(Program& program, Zone zone, std::function<void(VoxelData&)> = nullptr) = 0;
    const std::deque<VoxelData>& GetResults();
    void SaveDataToFile(std::string file);
    void LoadDataFromFile(std::string file);
    void SetVoxelColor(Color color);
    Color GetVoxelColor();

protected:
    std::deque<VoxelData>* m_results;
    Color baseColor{1, 1, 1, 0.2};

    bool CheckZone(Zone zone, ZoneFlags flags) const;
    ZoneFlags GetZoneFlags(const std::vector<std::pair<Vector3, double> > &values);
};

#endif // BASECALCULATOR_H
