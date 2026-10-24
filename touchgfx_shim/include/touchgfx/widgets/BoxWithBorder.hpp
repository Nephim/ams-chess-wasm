#ifndef TOUCHGFX_BOXWITHBORDER_HPP
#define TOUCHGFX_BOXWITHBORDER_HPP

#include <touchgfx/widgets/Box.hpp>

namespace touchgfx
{

class BoxWithBorder : public Box
{
public:
    BoxWithBorder()
        : Box(), borderColor(0), borderSize(0)
    {}

    BoxWithBorder(uint16_t width, uint16_t height, colortype color, colortype borderColor, uint8_t borderSize, uint8_t alpha = 255)
        : Box(width, height, color, alpha), borderColor(borderColor), borderSize(borderSize)
    {}

    void setBorderColor(colortype c) { borderColor = c; }
    colortype getBorderColor() const { return borderColor; }

    void setBorderSize(uint8_t size) { borderSize = size; }
    uint8_t getBorderSize() const { return borderSize; }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!visible || alpha == 0 || parentAlpha == 0) return;
        uint8_t effAlpha = static_cast<uint8_t>((static_cast<uint16_t>(alpha) * parentAlpha) / 255);
        int16_t ax = parentAbsX + rect.x;
        int16_t ay = parentAbsY + rect.y;

        if (borderSize > 0)
        {
            SDLBackend::fillRect(ax, ay, rect.width, rect.height, borderColor, effAlpha);
            int16_t innerW = rect.width - 2 * borderSize;
            int16_t innerH = rect.height - 2 * borderSize;
            if (innerW > 0 && innerH > 0)
            {
                SDLBackend::fillRect(ax + borderSize, ay + borderSize, innerW, innerH, color, effAlpha);
            }
        }
        else
        {
            SDLBackend::fillRect(ax, ay, rect.width, rect.height, color, effAlpha);
        }
    }

protected:
    colortype borderColor;
    uint8_t borderSize;
};

} // namespace touchgfx

#endif // TOUCHGFX_BOXWITHBORDER_HPP
