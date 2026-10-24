#ifndef TOUCHGFX_LCD24BPP_HPP
#define TOUCHGFX_LCD24BPP_HPP

#include <touchgfx/lcd/LCD.hpp>

namespace touchgfx
{

class LCD24bpp : public LCD
{
public:
    void enableTextureMapperRGB888() {}
    void enableTextureMapperARGB8888() {}
    void enableTextureMapperL8_RGB888() {}
    void enableTextureMapperL8_ARGB8888_NearestNeighbor() {}
    void enableTextureMapperA4() {}
    void enableDecompressorL8_All() {}
};

} // namespace touchgfx

#endif // TOUCHGFX_LCD24BPP_HPP
