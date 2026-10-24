#ifndef TOUCHGFX_COLOR_HPP
#define TOUCHGFX_COLOR_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{

class Color
{
public:
    static inline colortype getColorFromRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        return (static_cast<uint32_t>(r) << 16) |
               (static_cast<uint32_t>(g) << 8) |
               static_cast<uint32_t>(b);
    }

    static inline colortype getColorFrom24BitRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        return getColorFromRGB(r, g, b);
    }

    static inline uint8_t getRed(colortype color)
    {
        return static_cast<uint8_t>((color >> 16) & 0xFF);
    }

    static inline uint8_t getGreen(colortype color)
    {
        return static_cast<uint8_t>((color >> 8) & 0xFF);
    }

    static inline uint8_t getBlue(colortype color)
    {
        return static_cast<uint8_t>(color & 0xFF);
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_COLOR_HPP
