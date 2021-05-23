#ifndef IMAGERECURSIVECALCULATOR_H
#define IMAGERECURSIVECALCULATOR_H

#include "ImageCalculator.h"
#include "matrix5.h"

class ImageMatrixCalculator: public ImageCalculator
{
public:
    ImageMatrixCalculator(Vector3i step);
    virtual const std::deque<ImageData>& Calculate(Program& program, ImageType type, std::function<void(ImageData&)> iterFunc = nullptr);

protected:
    Vector3i m_step;

    const std::deque<ImageData>& matrix1(Program& program, ImageType type, std::function<void(ImageData&)> iterFunc);
    const std::deque<ImageData>& matrix2(Program& program, ImageType type, std::function<void(ImageData&)> iterFunc);
    const std::deque<ImageData>& matrix3(Program& program, ImageType type, std::function<void(ImageData&)> iterFunc);

};

#endif // IMAGERECURSIVECALCULATOR_H
