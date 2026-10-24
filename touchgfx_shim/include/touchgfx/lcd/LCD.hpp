#ifndef TOUCHGFX_LCD_HPP
#define TOUCHGFX_LCD_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Font.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdarg.h>

namespace touchgfx
{

class LCD
{
public:
    struct StringVisuals
    {
        const Font* font;
        Alignment alignment;
        colortype color;
        int16_t indentation;
        int16_t linespace;
        uint8_t alpha;
        TextDirection textDirection;
        WideTextAction wideTextAction;

        StringVisuals()
            : font(nullptr), alignment(LEFT), color(0), indentation(0),
              linespace(0), alpha(255), textDirection(TEXT_DIRECTION_LTR),
              wideTextAction(WIDE_TEXT_NONE)
        {}
    };

    virtual ~LCD() {}

    void drawString(Rect widgetArea, const Rect& invalidatedArea, const StringVisuals& stringVisuals, const Unicode::UnicodeChar* format, ...);

protected:
    void drawStringRTL(Rect /*widgetArea*/, const Rect& /*invalidatedArea*/, const StringVisuals& /*stringVisuals*/, const Unicode::UnicodeChar* /*format*/, va_list /*pArg*/) {}
};

} // namespace touchgfx

#endif // TOUCHGFX_LCD_HPP
