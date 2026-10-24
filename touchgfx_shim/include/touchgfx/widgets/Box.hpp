#ifndef TOUCHGFX_BOX_HPP
#define TOUCHGFX_BOX_HPP

#include <touchgfx/Drawable.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/SDLBackend.hpp>

namespace touchgfx
{

class Box : public Drawable
{
public:
    Box()
        : Drawable(), color(0), alpha(255)
    {}

    Box(uint16_t width, uint16_t height, colortype color, uint8_t alpha = 255)
        : Drawable(), color(color), alpha(alpha)
    {
        setWidthHeight(width, height);
    }

    virtual ~Box() {}

    void setColor(colortype c) { color = c; }
    colortype getColor() const { return color; }

    void setAlpha(uint8_t a) { alpha = a; }
    uint8_t getAlpha() const { return alpha; }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!visible || alpha == 0 || parentAlpha == 0) return;
        uint8_t effAlpha = static_cast<uint8_t>((static_cast<uint16_t>(alpha) * parentAlpha) / 255);
        SDLBackend::fillRect(parentAbsX + rect.x, parentAbsY + rect.y, rect.width, rect.height, color, effAlpha);
    }

protected:
    colortype color;
    uint8_t alpha;
};

} // namespace touchgfx

#endif // TOUCHGFX_BOX_HPP
