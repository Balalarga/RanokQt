#include "LinearGradientModel.h"


LinearGradientModel::LinearGradientModel(QVector<QColor> colors,
                                         QObject *parent):
    QObject(parent),
    _stops(colors)
{

}

QColor LinearGradientModel::GetColor(unsigned value)
{
    // Find the "bin" that value falls in
    unsigned range = UINT_MAX;
    float step = range / (float)(_stops.size()-1);
    int bin = (int)(value / step);

    // Normalize value in the interval (0,1]
    float normalized_v = (value - bin*step) / step;

    return interpolate(_stops[bin], _stops[bin+1], normalized_v);
}

QColor LinearGradientModel::interpolate(QColor c1, QColor c2,
                                        float normalized_value)
{
    if( normalized_value <= 0.0 ){ return c1; }
    if( normalized_value >= 1.0 ){ return c2; }

    uint8_t red = (uint8_t)((1.0-normalized_value)*c1.red() +
                            normalized_value*c2.red());
    uint8_t green = (uint8_t)((1.0-normalized_value)*c1.green() +
                              normalized_value*c2.green());
    uint8_t blue = (uint8_t)((1.0-normalized_value)*c1.blue() +
                             normalized_value*c2.blue());


    uint8_t alpha = (uint8_t)((1.0-normalized_value)*c1.alpha() +
                             normalized_value*c2.alpha());

    return QColor{red, green, blue, alpha};
}
