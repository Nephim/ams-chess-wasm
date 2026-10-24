#ifndef TOUCHGFX_TEXTAREA_HPP
#define TOUCHGFX_TEXTAREA_HPP

#include <touchgfx/Drawable.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/SDLBackend.hpp>

namespace touchgfx
{

class TextArea : public Drawable
{
public:
    TextArea()
        : Drawable(), typedText(TYPED_TEXT_INVALID), color(0), linespace(0),
          alpha(255), indentation(0), rotation(0), wideTextAction(WIDE_TEXT_NONE)
    {}

    virtual ~TextArea() {}

    void setTypedText(const TypedText& t)
    {
        typedText = t;
        if (rect.width == 0 && rect.height == 0)
        {
            resizeToCurrentText();
        }
    }

    TypedText getTypedText() const { return typedText; }

    void setColor(colortype c) { color = c; }
    colortype getColor() const { return color; }

    void setLinespacing(int16_t space) { linespace = space; }
    int16_t getLinespacing() const { return linespace; }

    void setIndentation(uint8_t indent) { indentation = indent; }
    uint8_t getIndentation() const { return indentation; }

    void setAlpha(uint8_t a) { alpha = a; }
    uint8_t getAlpha() const { return alpha; }

    void setWideTextAction(WideTextAction action) { wideTextAction = action; }
    WideTextAction getWideTextAction() const { return wideTextAction; }

    virtual void resizeToCurrentText()
    {
        int16_t w = 0, h = 0;
        SDLBackend::measureText(typedText, nullptr, nullptr, w, h);
        if (w > 0) rect.width = w;
        if (h > 0) rect.height = h;
    }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!visible || alpha == 0 || parentAlpha == 0 || !typedText.hasValidId()) return;
        uint8_t effAlpha = static_cast<uint8_t>((static_cast<uint16_t>(alpha) * parentAlpha) / 255);
        SDLBackend::drawText(typedText, nullptr, nullptr,
                             parentAbsX + rect.x, parentAbsY + rect.y,
                             rect.width, rect.height, color, effAlpha, false);
    }

protected:
    TypedText typedText;
    colortype color;
    int16_t linespace;
    uint8_t alpha;
    uint8_t indentation;
    uint8_t rotation;
    WideTextAction wideTextAction;
};

} // namespace touchgfx

#endif // TOUCHGFX_TEXTAREA_HPP
