#ifndef TOUCHGFX_CLICKEVENT_HPP
#define TOUCHGFX_CLICKEVENT_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{

class ClickEvent
{
public:
    enum ClickEventType
    {
        PRESSED,
        RELEASED,
        CANCEL
    };

    ClickEvent(ClickEventType type, int16_t x, int16_t y, int16_t force = 1)
        : type(type), x(x), y(y), force(force)
    {
    }

    int16_t getX() const { return x; }
    int16_t getY() const { return y; }
    void setX(int16_t newX) { x = newX; }
    void setY(int16_t newY) { y = newY; }
    ClickEventType getType() const { return type; }
    int16_t getForce() const { return force; }

private:
    ClickEventType type;
    int16_t x;
    int16_t y;
    int16_t force;
};

} // namespace touchgfx

#endif // TOUCHGFX_CLICKEVENT_HPP
