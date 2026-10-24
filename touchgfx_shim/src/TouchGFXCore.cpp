#include <touchgfx/Drawable.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/Application.hpp>
#include <touchgfx/SDLBackend.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/FontManager.hpp>
#include <fonts/GeneratedFont.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include <images/BitmapDatabase.hpp>
#include "fatfs.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace touchgfx
{

Application* Application::instance = nullptr;

Application::Application()
{
    instance = this;
}

Drawable::~Drawable()
{
    if (parent)
    {
        parent->remove(*this);
        parent = nullptr;
    }
    if (Application::getInstance())
    {
        Application::getInstance()->unregisterTimerWidget(this);
    }
}

void Drawable::invalidate() const
{
    SDLBackend::markDirty();
}

Rect Drawable::getAbsoluteRect() const
{
    int16_t absX = rect.x;
    int16_t absY = rect.y;
    const Drawable* p = parent;
    while (p)
    {
        absX += p->getX();
        absY += p->getY();
        p = p->getParent();
    }
    return Rect(absX, absY, rect.width, rect.height);
}

static HAL s_halInstance;
static LCD24bpp s_lcdInstance;

HAL* HAL::getInstance()
{
    return &s_halInstance;
}

LCD& HAL::lcd()
{
    return s_lcdInstance;
}

const TypedText::TypedTextData* TypedText::typedTexts = nullptr;
const Font* const* TypedText::fonts = nullptr;
uint16_t TypedText::numberOfTypedTexts = 0;
Texts TypedText::texts;

FontProvider* FontManager::fontProvider = nullptr;

uint16_t Font::getStringWidthRTL(TextDirection /*textDirection*/, const Unicode::UnicodeChar* text, va_list /*pArg*/) const
{
    if (!text) return 0;
    std::string utf8 = Unicode::toUTF8(text);
    return static_cast<uint16_t>(utf8.size() * (fontHeight / 2));
}

uint16_t Bitmap::getWidth() const
{
    return SDLBackend::getBitmapWidth(bitmapId);
}

uint16_t Bitmap::getHeight() const
{
    return SDLBackend::getBitmapHeight(bitmapId);
}

void Bitmap::registerBitmapDatabase(const BitmapData* /*data*/, const uint16_t /*n*/, BitmapData* /*cache*/, uint32_t /*csize*/, uint32_t /*numberOfDynamicBitmaps*/)
{
}

// GeneratedFont & GeneratedVectorFont implementations for TypedTextDatabase.cpp
GeneratedFont::GeneratedFont(const GlyphNode* glyphs, uint16_t numGlyphs, uint16_t height, uint16_t baseline,
                             uint8_t pixAboveTop, uint8_t pixBelowBottom, uint8_t bitsPerPixel, uint8_t byteAlignRow,
                             uint8_t maxLeft, uint8_t maxRight, const uint8_t* const* glyphDataInternalFlash,
                             const KerningNode* kerningList, const Unicode::UnicodeChar fallbackChar,
                             const Unicode::UnicodeChar ellipsisChar, const uint16_t* const gsubData,
                             const FontContextualFormsTable* formsTable)
    : ConstFont(glyphs, numGlyphs, height, baseline, pixAboveTop, pixBelowBottom, bitsPerPixel, byteAlignRow,
                maxLeft, maxRight, fallbackChar, ellipsisChar),
      glyphData(glyphDataInternalFlash),
      kerningData(kerningList),
      gsubTable(gsubData),
      arabicTable(formsTable)
{}

const uint8_t* GeneratedFont::getPixelData(const GlyphNode* /*glyph*/) const
{
    return nullptr;
}

int8_t GeneratedFont::getKerning(Unicode::UnicodeChar /*prevChar*/, const GlyphNode* /*glyph*/) const
{
    return 0;
}

GlyphNode GeneratedVectorFont::glyphNode = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

GeneratedVectorFont::GeneratedVectorFont(uint16_t baseline, float scale, const VectorFontNode* vectorGlyphs,
                                         const VectorFontData& fontData, const uint16_t* const* glyphData,
                                         const VectorKerningNode* kerningTable, const uint16_t* const gsubData,
                                         const FontContextualFormsTable* formsTable)
    : Font(baseline, static_cast<uint8_t>(fontData.maxBelowBottom), 4, 0,
           static_cast<uint8_t>(fontData.maxLeft), static_cast<uint8_t>(fontData.maxRight),
           fontData.fallbackChar, fontData.ellipsisChar,
           static_cast<uint8_t>(fontData.maxAboveTop), baseline),
      scaleFactor(scale),
      vectorNodes(vectorGlyphs),
      vectorTable(glyphData),
      kerningTable(kerningTable),
      gsubTable(gsubData),
      arabicTable(formsTable),
      numberOfGlyphs(fontData.numberOfGlyphs)
{}

const GlyphNode* GeneratedVectorFont::getGlyph(Unicode::UnicodeChar /*unicode*/) const
{
    return nullptr;
}

const GlyphNode* GeneratedVectorFont::getGlyph(Unicode::UnicodeChar /*unicode*/, const uint8_t*& pixelData, uint8_t& bpp) const
{
    pixelData = nullptr;
    bpp = 4;
    return nullptr;
}

int8_t GeneratedVectorFont::getKerning(Unicode::UnicodeChar /*prevChar*/, const GlyphNode* /*glyph*/) const
{
    return 0;
}

} // namespace touchgfx

// Font instances required by TypedTextDatabase.cpp (sized according to ApplicationFontProvider.hpp: 12px, 40px, 10px, 20px, 10px)
touchgfx::GeneratedVectorFont& getFont_vector_verdana_12()
{
    static const touchgfx::VectorFontData data12 = {0, 12, 0, 0, 0, 2, 0, 0};
    static touchgfx::GeneratedVectorFont font12(12, 1.0f, nullptr, data12, nullptr, nullptr, nullptr, nullptr);
    return font12;
}

touchgfx::GeneratedFont& getFont_verdana_40_4bpp()
{
    static touchgfx::GeneratedFont font40(nullptr, 0, 40, 40, 0, 8, 4, 0, 0, 0, nullptr, nullptr, 0, 0, nullptr, nullptr);
    return font40;
}

touchgfx::GeneratedFont& getFont_verdana_10_4bpp()
{
    static touchgfx::GeneratedFont font10(nullptr, 0, 10, 10, 0, 2, 4, 0, 0, 0, nullptr, nullptr, 0, 0, nullptr, nullptr);
    return font10;
}

touchgfx::GeneratedFont& getFont_verdana_20_4bpp()
{
    static touchgfx::GeneratedFont font20(nullptr, 0, 20, 20, 0, 4, 4, 0, 0, 0, nullptr, nullptr, 0, 0, nullptr, nullptr);
    return font20;
}

touchgfx::GeneratedVectorFont& getFont_vector_verdana_10()
{
    static const touchgfx::VectorFontData data10 = {0, 10, 0, 0, 0, 2, 0, 0};
    static touchgfx::GeneratedVectorFont fontVec10(10, 1.0f, nullptr, data10, nullptr, nullptr, nullptr, nullptr);
    return fontVec10;
}

touchgfx::Font* ApplicationFontProvider::getFont(touchgfx::FontId typography)
{
    switch (typography)
    {
    case Typography::DEFAULT:
        return &(getFont_vector_verdana_12());
    case Typography::LARGE:
        return &(getFont_verdana_40_4bpp());
    case Typography::SMALL:
        return &(getFont_verdana_10_4bpp());
    case Typography::TESTBOX:
        return &(getFont_verdana_20_4bpp());
    case Typography::HEADER:
        return &(getFont_vector_verdana_10());
    default:
        return &(getFont_vector_verdana_12());
    }
}

namespace BitmapDatabase
{
const touchgfx::Bitmap::BitmapData* getInstance()
{
    return nullptr;
}

uint16_t getInstanceSize()
{
    return 31;
}
} // namespace BitmapDatabase

// Persistent SD Card shim backed by browser localStorage in WebAssembly
static std::string s_inMemorySavedGame;

void WriteToSDCARD(const char* data)
{
    if (!data) return;
    s_inMemorySavedGame = data;
#ifdef __EMSCRIPTEN__
    EM_ASM({
        try {
            var jsonStr = UTF8ToString($0);
            window.localStorage.setItem('ams_chess_saved_state', jsonStr);
        } catch (e) {}
    }, data);
#endif
}

std::string ReadFromSDCARD()
{
#ifdef __EMSCRIPTEN__
    char* str = (char*)EM_ASM_PTR({
        try {
            var saved = window.localStorage.getItem('ams_chess_saved_state');
            if (saved && saved.length > 0) {
                var len = lengthBytesUTF8(saved) + 1;
                var buf = _malloc(len);
                stringToUTF8(saved, buf, len);
                return buf;
            }
        } catch (e) {}
        return 0;
    });
    if (str)
    {
        std::string result(str);
        free(str);
        s_inMemorySavedGame = result;
        return result;
    }
#endif
    return s_inMemorySavedGame;
}
