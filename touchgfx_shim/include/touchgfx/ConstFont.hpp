#ifndef TOUCHGFX_CONSTFONT_HPP
#define TOUCHGFX_CONSTFONT_HPP

#include <touchgfx/Font.hpp>

namespace touchgfx
{

class ConstFont : public Font
{
public:
    ConstFont(const GlyphNode* list, uint16_t size, uint16_t height, uint16_t baseline,
              uint8_t pixAboveTop, uint8_t pixBelowBottom, uint8_t bitsPerPixel,
              uint8_t byteAlignRow, uint8_t maxLeft, uint8_t maxRight,
              const Unicode::UnicodeChar fallbackChar, const Unicode::UnicodeChar ellipsisChar)
        : Font(height, pixBelowBottom, bitsPerPixel, byteAlignRow, maxLeft, maxRight,
               fallbackChar, ellipsisChar, pixAboveTop, baseline),
          glyphList(list), listSize(size)
    {}

    using Font::getGlyph;

    virtual const GlyphNode* getGlyph(Unicode::UnicodeChar unicode, const uint8_t*& pixelData, uint8_t& bpp) const override
    {
        pixelData = nullptr;
        bpp = bitsPerPixel;
        return find(unicode);
    }

    virtual const uint8_t* getPixelData(const GlyphNode* /*glyph*/) const = 0;

    const GlyphNode* find(Unicode::UnicodeChar /*unicode*/) const { return nullptr; }

protected:
    const GlyphNode* glyphList;
    uint16_t listSize;
};

} // namespace touchgfx

#endif // TOUCHGFX_CONSTFONT_HPP
