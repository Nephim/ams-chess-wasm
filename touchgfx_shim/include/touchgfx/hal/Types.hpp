#ifndef TOUCHGFX_TYPES_HPP
#define TOUCHGFX_TYPES_HPP

#include <stdint.h>
#include <stddef.h>
#include <algorithm>

#ifndef TEXT_LOCATION_FLASH_PRAGMA
#define TEXT_LOCATION_FLASH_PRAGMA
#endif

#ifndef TEXT_LOCATION_FLASH_ATTRIBUTE
#define TEXT_LOCATION_FLASH_ATTRIBUTE
#endif

#ifndef KEEP
#define KEEP
#endif

namespace touchgfx
{

typedef uint32_t colortype;
typedef uint16_t BitmapId;
typedef uint16_t FontId;
typedef uint16_t TypedTextId;
typedef uint16_t LanguageId;

enum Alignment
{
    LEFT,
    CENTER,
    RIGHT
};

enum TextDirection
{
    TEXT_DIRECTION_LTR,
    TEXT_DIRECTION_RTL
};

enum WideTextAction
{
    WIDE_TEXT_NONE,
    WIDE_TEXT_WORDWRAP,
    WIDE_TEXT_WORDWRAP_ELLIPSIS,
    WIDE_TEXT_CHARWRAP
};

enum DisplayOrientation
{
    ORIENTATION_LANDSCAPE,
    ORIENTATION_PORTRAIT
};

struct Rect
{
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;

    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(int16_t x_, int16_t y_, int16_t w_, int16_t h_)
        : x(x_), y(y_), width(w_), height(h_) {}

    int16_t right() const { return x + width; }
    int16_t bottom() const { return y + height; }
    bool isEmpty() const { return width <= 0 || height <= 0; }

    bool intersect(int16_t px, int16_t py) const
    {
        return px >= x && px < (x + width) && py >= y && py < (y + height);
    }

    Rect operator&(const Rect& other) const
    {
        int16_t nx = std::max(x, other.x);
        int16_t ny = std::max(y, other.y);
        int16_t nr = std::min(right(), other.right());
        int16_t nb = std::min(bottom(), other.bottom());
        if (nr <= nx || nb <= ny)
        {
            return Rect(0, 0, 0, 0);
        }
        return Rect(nx, ny, nr - nx, nb - ny);
    }
};

} // namespace touchgfx

#endif // TOUCHGFX_TYPES_HPP
