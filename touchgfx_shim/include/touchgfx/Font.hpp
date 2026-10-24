#ifndef TOUCHGFX_FONT_HPP
#define TOUCHGFX_FONT_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdarg.h>

namespace touchgfx
{

struct GlyphNode
{
    uint32_t dataOffset;
    Unicode::UnicodeChar unicode;
    uint8_t width;
    uint8_t height;
    int8_t top;
    int8_t left;
    uint8_t advance;
    uint8_t kerningTablePos;
    uint8_t kerningTableSize;
    uint8_t flags;
};

struct KerningNode
{
    Unicode::UnicodeChar unicodePrevChar;
    int8_t distance;
};

struct FontContextualFormsTable
{
    const Unicode::UnicodeChar (*contextualForms4Long)[5];
    const Unicode::UnicodeChar (*contextualForms3Long)[5];
    const Unicode::UnicodeChar (*contextualForms2Long)[5];
    const Unicode::UnicodeChar (*contextualForms0621_063a)[4];
    const Unicode::UnicodeChar (*contextualForms0641_064a)[4];
    const Unicode::UnicodeChar (*contextualForms06XX)[5];
    uint16_t contextualForms4LongSize;
    uint16_t contextualForms3LongSize;
    uint16_t contextualForms2LongSize;
    uint16_t contextualForms06XXSize;
};

class Font
{
public:
    Font()
        : fontHeight(12), baseline(10), pixAboveTop(0), pixBelowBottom(2),
          bitsPerPixel(4), byteAlignRow(0), maxLeft(0), maxRight(0),
          fallbackChar(0), ellipsisChar(0)
    {}

    Font(uint16_t height, uint8_t pixBelowBase, uint8_t bitsPerPixel, uint8_t byteAlignRow,
         uint8_t maxLeft, uint8_t maxRight, const Unicode::UnicodeChar fallbackChar = 0,
         const Unicode::UnicodeChar ellipsisChar = 0, uint8_t pixAboveTop = 0, uint16_t baseline = 0)
        : fontHeight(height), baseline(baseline ? baseline : height), pixAboveTop(pixAboveTop),
          pixBelowBottom(pixBelowBase), bitsPerPixel(bitsPerPixel), byteAlignRow(byteAlignRow),
          maxLeft(maxLeft), maxRight(maxRight), fallbackChar(fallbackChar), ellipsisChar(ellipsisChar)
    {}

    virtual ~Font() {}

    virtual const GlyphNode* getGlyph(Unicode::UnicodeChar unicode) const
    {
        const uint8_t* dummyPixelData = nullptr;
        uint8_t dummyBpp = 0;
        return getGlyph(unicode, dummyPixelData, dummyBpp);
    }

    virtual const GlyphNode* getGlyph(Unicode::UnicodeChar /*unicode*/, const uint8_t*& pixelData, uint8_t& bpp) const
    {
        pixelData = nullptr;
        bpp = bitsPerPixel;
        return nullptr;
    }

    virtual int8_t getKerning(Unicode::UnicodeChar /*prevChar*/, const GlyphNode* /*glyph*/) const { return 0; }
    virtual bool isVectorBasedFont() const { return false; }
    virtual float getScaleFactor() const { return 1.0f; }
    virtual const uint16_t* getGSUBTable() const { return nullptr; }
    virtual const FontContextualFormsTable* getContextualFormsTable() const { return nullptr; }

    virtual uint16_t getFontHeight() const { return fontHeight; }
    virtual uint16_t getMinimumTextHeight() const { return fontHeight + pixBelowBottom; }
    virtual uint8_t getBitsPerPixel() const { return bitsPerPixel; }

    uint16_t getStringWidth(const Unicode::UnicodeChar* text, ...) const;
    uint16_t getStringWidth(TextDirection textDirection, const Unicode::UnicodeChar* text, ...) const;

protected:
    uint16_t getStringWidthRTL(TextDirection textDirection, const Unicode::UnicodeChar* text, va_list pArg) const;

    uint16_t fontHeight;
    uint16_t baseline;
    uint8_t pixAboveTop;
    uint8_t pixBelowBottom;
    uint8_t bitsPerPixel;
    uint8_t byteAlignRow;
    uint8_t maxLeft;
    uint8_t maxRight;
    Unicode::UnicodeChar fallbackChar;
    Unicode::UnicodeChar ellipsisChar;
};

} // namespace touchgfx

#endif // TOUCHGFX_FONT_HPP
