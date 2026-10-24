#ifndef TOUCHGFX_IMAGE_HPP
#define TOUCHGFX_IMAGE_HPP

#include <touchgfx/Drawable.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/SDLBackend.hpp>

namespace touchgfx
{

class Image : public Drawable
{
public:
    Image(const Bitmap& bmp = Bitmap(BITMAP_INVALID))
        : Drawable(), bitmap(bmp), alpha(255)
    {
        if (bmp.getId() != BITMAP_INVALID)
        {
            setWidthHeight(bmp.getWidth(), bmp.getHeight());
        }
    }

    virtual ~Image() {}

    virtual void setBitmap(const Bitmap& bmp)
    {
        bitmap = bmp;
        if (bmp.getId() != BITMAP_INVALID)
        {
            setWidthHeight(bmp.getWidth(), bmp.getHeight());
        }
    }

    Bitmap getBitmap() const { return bitmap; }
    BitmapId getBitmapId() const { return bitmap.getId(); }

    void setAlpha(uint8_t a) { alpha = a; }
    uint8_t getAlpha() const { return alpha; }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!visible || alpha == 0 || parentAlpha == 0 || bitmap.getId() == BITMAP_INVALID) return;
        uint8_t effAlpha = static_cast<uint8_t>((static_cast<uint16_t>(alpha) * parentAlpha) / 255);
        SDLBackend::drawBitmap(bitmap.getId(), parentAbsX + rect.x, parentAbsY + rect.y, rect.width, rect.height, effAlpha);
    }

protected:
    Bitmap bitmap;
    uint8_t alpha;
};

} // namespace touchgfx

#endif // TOUCHGFX_IMAGE_HPP
