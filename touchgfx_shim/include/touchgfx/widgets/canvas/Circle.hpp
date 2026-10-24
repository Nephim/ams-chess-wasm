#ifndef TOUCHGFX_CIRCLE_HPP
#define TOUCHGFX_CIRCLE_HPP

#include <touchgfx/Drawable.hpp>

namespace touchgfx
{

class Circle : public Drawable
{
public:
    Circle() : Drawable() {}
    virtual ~Circle() {}

    virtual void draw(int16_t /*parentAbsX*/, int16_t /*parentAbsY*/, uint8_t /*parentAlpha*/ = 255) const override {}
};

} // namespace touchgfx

#endif // TOUCHGFX_CIRCLE_HPP
