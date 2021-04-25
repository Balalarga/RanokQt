#ifndef BASECALCULATOR_H
#define BASECALCULATOR_H

#include <deque>
#include <vector>
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

class BaseCalculator
{
public:
    BaseCalculator(){};
    virtual ~BaseCalculator(){};
    virtual std::deque<VoxelData> Calculate(Program& program, Zone zone) = 0;

protected:
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
