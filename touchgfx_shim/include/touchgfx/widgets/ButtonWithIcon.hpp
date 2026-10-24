#ifndef TOUCHGFX_BUTTONWITHICON_HPP
#define TOUCHGFX_BUTTONWITHICON_HPP

#include <touchgfx/widgets/AbstractButton.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/SDLBackend.hpp>

namespace touchgfx
{

class ButtonWithIcon : public AbstractButton
{
public:
    ButtonWithIcon()
        : AbstractButton(), iconX(0), iconY(0)
    {}

    void setBitmaps(const Bitmap& newBackgroundReleased, const Bitmap& newBackgroundPressed,
                    const Bitmap& newIconReleased, const Bitmap& newIconPressed)
    {
        up = newBackgroundReleased;
        down = newBackgroundPressed;
        iconReleased = newIconReleased;
        iconPressed = newIconPressed;
        setWidthHeight(up.getWidth(), up.getHeight());
    }

    void setIconXY(int16_t x, int16_t y)
    {
        iconX = x;
        iconY = y;
    }

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

        const Bitmap& ic = pressed ? iconPressed : iconReleased;
        if (ic.getId() != BITMAP_INVALID)
        {
            SDLBackend::drawBitmap(ic.getId(), ax + iconX, ay + iconY, ic.getWidth(), ic.getHeight(), effAlpha);
        }
    }

protected:
    Bitmap up;
    Bitmap down;
    Bitmap iconReleased;
    Bitmap iconPressed;
    int16_t iconX;
    int16_t iconY;
};

} // namespace touchgfx

#endif // TOUCHGFX_BUTTONWITHICON_HPP
