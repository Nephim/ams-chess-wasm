#ifndef TOUCHGFX_DRAWABLE_HPP
#define TOUCHGFX_DRAWABLE_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/events/ClickEvent.hpp>

namespace touchgfx
{

class Container;

class Drawable
{
public:
    Drawable()
        : rect(0, 0, 0, 0), parent(nullptr), visible(true), touchable(false)
    {}

    Drawable(const Drawable& other)
        : rect(other.rect), parent(nullptr), visible(other.visible), touchable(other.touchable)
    {}

    Drawable& operator=(const Drawable& other)
    {
        if (this != &other)
        {
            rect = other.rect;
            visible = other.visible;
            touchable = other.touchable;
        }
        return *this;
    }

    virtual ~Drawable();

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const = 0;

    virtual void handleClickEvent(const ClickEvent& /*event*/) {}
    virtual void handleDragEvent(int16_t /*oldX*/, int16_t /*oldY*/, int16_t /*newX*/, int16_t /*newY*/) {}
    virtual void handleTickEvent() {}

    virtual Drawable* findTouchable(int16_t localX, int16_t localY, int16_t& outRelX, int16_t& outRelY)
    {
        if (!visible) return nullptr;
        if (touchable && localX >= 0 && localX < rect.width && localY >= 0 && localY < rect.height)
        {
            outRelX = localX;
            outRelY = localY;
            return this;
        }
        return nullptr;
    }

    void setX(int16_t x) { rect.x = x; }
    void setY(int16_t y) { rect.y = y; }
    void setXY(int16_t x, int16_t y) { rect.x = x; rect.y = y; }
    void setWidth(int16_t width) { rect.width = width; }
    void setHeight(int16_t height) { rect.height = height; }
    void setWidthHeight(int16_t width, int16_t height) { rect.width = width; rect.height = height; }
    void setPosition(int16_t x, int16_t y, int16_t width, int16_t height)
    {
        rect.x = x;
        rect.y = y;
        rect.width = width;
        rect.height = height;
    }

    void moveTo(int16_t x, int16_t y)
    {
        rect.x = x;
        rect.y = y;
        invalidate();
    }

    int16_t getX() const { return rect.x; }
    int16_t getY() const { return rect.y; }
    int16_t getWidth() const { return rect.width; }
    int16_t getHeight() const { return rect.height; }
    const Rect& getRect() const { return rect; }

    Rect getAbsoluteRect() const;

    void setVisible(bool vis) { visible = vis; }
    bool isVisible() const { return visible; }

    void setTouchable(bool touch) { touchable = touch; }
    bool isTouchable() const { return touchable; }

    void setParent(Container* p) { parent = p; }
    Container* getParent() const { return parent; }

    virtual void invalidate() const;

protected:
    Rect rect;
    Container* parent;
    bool visible;
    bool touchable;
};

} // namespace touchgfx

#endif // TOUCHGFX_DRAWABLE_HPP
