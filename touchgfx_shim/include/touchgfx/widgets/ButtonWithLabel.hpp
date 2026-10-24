#ifndef TOUCHGFX_BUTTONWITHLABEL_HPP
#define TOUCHGFX_BUTTONWITHLABEL_HPP

#include <touchgfx/widgets/AbstractButton.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/SDLBackend.hpp>

namespace touchgfx
{

class ButtonWithLabel : public AbstractButton
{
public:
    ButtonWithLabel()
        : AbstractButton(), color(0), colorPressed(0)
    {}

    void setBitmaps(const Bitmap& newBackgroundReleased, const Bitmap& newBackgroundPressed)
    {
        up = newBackgroundReleased;
        down = newBackgroundPressed;
        setWidthHeight(up.getWidth(), up.getHeight());
    }

    void setLabelText(TypedText t) { typedText = t; }
    TypedText getLabelText() const { return typedText; }

    void setLabelColor(colortype col) { color = col; }
    void setLabelColorPressed(colortype col) { colorPressed = col; }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!visible || alpha == 0 || parentAlpha == 0) return;
        uint8_t effAlpha = static_cast<uint8_t>((static_cast<uint16_t>(alpha) * parentAlpha) / 255);
        int16_t ax = parentAbsX + rect.x;
        int16_t ay = parentAbsY + rect.y;

        const Bitmap& bg = pressed ? down : up;
        if (bg.getId() != BITMAP_INVALID)
        {
            SDLBackend::drawBitmap(bg.getId(), ax, ay, bg.getWidth(), bg.getHeight(), effAlpha);
        }

        if (typedText.hasValidId())
        {
            colortype textCol = pressed ? colorPressed : color;
            SDLBackend::drawText(typedText, nullptr, nullptr, ax, ay, rect.width, rect.height, textCol, effAlpha, true);
        }
    }

protected:
    Bitmap up;
    Bitmap down;
    TypedText typedText;
    colortype color;
    colortype colorPressed;
};

} // namespace touchgfx

#endif // TOUCHGFX_BUTTONWITHLABEL_HPP
