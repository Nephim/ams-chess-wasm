#ifndef TOUCHGFX_BITMAP_HPP
#define TOUCHGFX_BITMAP_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{

const BitmapId BITMAP_INVALID = 0xFFFFU;

class Bitmap
{
public:
    enum BitmapFormat
    {
        RGB565,
        RGB888,
        ARGB8888,
        BW,
        BW_RLE,
        GRAY2,
        GRAY4,
        ABGR2222,
        BGRA2222,
        RGAB2222,
        RGBA2222,
        L8,
        A4,
        CUSTOM
    };

    struct BitmapData
    {
        const uint8_t* data;
        const uint8_t* extraData;
        uint16_t width;
        uint16_t height;
        uint16_t solidRect_x;
        uint16_t solidRect_y;
        uint16_t solidRect_width;
        uint8_t format;
        uint16_t solidRect_height;
        uint8_t extra;
    };

    explicit Bitmap(const BitmapId id = BITMAP_INVALID) : bitmapId(id) {}

    BitmapId getId() const { return bitmapId; }
    uint16_t getWidth() const;
    uint16_t getHeight() const;
    Rect getRect() const { return Rect(0, 0, getWidth(), getHeight()); }

    bool operator==(const Bitmap& other) const { return bitmapId == other.bitmapId; }
    bool operator!=(const Bitmap& other) const { return bitmapId != other.bitmapId; }

    static void registerBitmapDatabase(const BitmapData* data, const uint16_t n, BitmapData* cache = nullptr, uint32_t csize = 0, uint32_t numberOfDynamicBitmaps = 0);

private:
    BitmapId bitmapId;
};

} // namespace touchgfx

#endif // TOUCHGFX_BITMAP_HPP
