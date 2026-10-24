#ifndef TOUCHGFX_EASINGEQUATIONS_HPP
#define TOUCHGFX_EASINGEQUATIONS_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{

typedef int16_t (*EasingEquation)(uint16_t, int16_t, int16_t, uint16_t);

class EasingEquations
{
public:
    static int16_t linearEaseNone(uint16_t t, int16_t b, int16_t c, uint16_t d)
    {
        if (d == 0) return b + c;
        return static_cast<int16_t>(b + (static_cast<int32_t>(c) * t) / d);
    }

    static int16_t quintEaseIn(uint16_t t, int16_t b, int16_t c, uint16_t d)
    {
        if (d == 0) return b + c;
        float tf = static_cast<float>(t) / static_cast<float>(d);
        return static_cast<int16_t>(b + c * (tf * tf * tf * tf * tf));
    }

    static int16_t quintEaseOut(uint16_t t, int16_t b, int16_t c, uint16_t d)
    {
        if (d == 0) return b + c;
        float tf = static_cast<float>(t) / static_cast<float>(d) - 1.0f;
        return static_cast<int16_t>(b + c * (tf * tf * tf * tf * tf + 1.0f));
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_EASINGEQUATIONS_HPP
