#ifndef BASECALCULATOR_H
#define BASECALCULATOR_H

#include <deque>
#include <vector>
#include <functional>
#include <glm/glm.hpp>

#include "Lang/program.h"

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
    VoxelData(const glm::vec3& center, const double& size,
              const glm::vec4& color, std::vector<std::pair<glm::vec3, double>>& values):
        center(center), size(size), color(color), values(values)
    {}
    glm::vec3 center;
    double size;
    glm::vec4 color;
    std::vector<std::pair<glm::vec3, double>> values;
};

struct StorageItem
{
    glm::vec3 point;
    friend inline bool operator<(const StorageItem& a, const StorageItem& b);
};

class BaseCalculator
{
public:
    BaseCalculator();
    virtual ~BaseCalculator();
    virtual const std::deque<VoxelData>& Calculate(Program& program, Zone zone) = 0;
    const std::deque<VoxelData>& GetResults();
    void SetIterationFunc(std::function<void(VoxelData&)> iterFunc);
    void SaveDataToFile(std::string file);
    void LoadDataFromFile(std::string file);

protected:
    std::deque<VoxelData>* m_results;
    std::map<StorageItem, double> storage;
    std::function<void(VoxelData&)> m_iterFunc;

    bool CheckZone(Zone zone, ZoneFlags flags) const;
    ZoneFlags GetZoneFlags(const std::vector<std::pair<glm::vec3, double> > &values);

};

#endif // BASECALCULATOR_H
