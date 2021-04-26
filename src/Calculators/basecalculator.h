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
    VoxelData(const glm::vec3& center, const glm::vec3& size, const glm::vec4& color):
        center(center), size(size), color(color)
    {}
    glm::vec3 center;
    glm::vec3 size;
    glm::vec4 color;
};

struct StorageItem
{
    glm::vec3 point;
    friend inline bool operator<(const StorageItem& a, const StorageItem& b);
};
inline bool operator<(const StorageItem& a, const StorageItem& b)
{
    return a.point.x < b.point.x && a.point.y < b.point.y && a.point.z < b.point.z;
}

class BaseCalculator
{
public:
    BaseCalculator(){m_results = new std::deque<VoxelData>;};
    virtual ~BaseCalculator(){delete m_results;};
    virtual const std::deque<VoxelData>& Calculate(Program& program, Zone zone) = 0;
    void SetIterationFunc(std::function<void(VoxelData&)> iterFunc){m_iterFunc = iterFunc;}

protected:
    std::deque<VoxelData>* m_results;
    std::map<StorageItem, double> storage;
    std::function<void(VoxelData&)> m_iterFunc;

    bool CheckZone(Zone zone, ZoneFlags flags) const
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
};

#endif // BASECALCULATOR_H
