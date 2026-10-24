#ifndef TOUCHGFX_TYPEDTEXT_HPP
#define TOUCHGFX_TYPEDTEXT_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Font.hpp>
#include <touchgfx/Texts.hpp>

namespace touchgfx
{

const TypedTextId TYPED_TEXT_INVALID = 0xFFFFU;

class TypedText
{
public:
    struct TypedTextData
    {
        uint8_t fontIdx;
        uint8_t alignment;
        uint8_t textDirection;
    };

    explicit TypedText(const TypedTextId id = TYPED_TEXT_INVALID) : typedTextId(id) {}

    TypedTextId getId() const { return typedTextId; }
    bool hasValidId() const { return typedTextId != TYPED_TEXT_INVALID && typedTextId < numberOfTypedTexts; }

    const Unicode::UnicodeChar* getText() const
    {
        if (!hasValidId()) return nullptr;
        return texts.getText(typedTextId);
    }

    const Font* getFont() const
    {
        if (!hasValidId() || !typedTexts || !fonts) return nullptr;
        return fonts[typedTexts[typedTextId].fontIdx];
    }

    FontId getFontId() const
    {
        if (!hasValidId() || !typedTexts) return 0;
        return typedTexts[typedTextId].fontIdx;
    }

    Alignment getAlignment() const
    {
        if (!hasValidId() || !typedTexts) return LEFT;
        return static_cast<Alignment>(typedTexts[typedTextId].alignment);
    }

    TextDirection getTextDirection() const
    {
        if (!hasValidId() || !typedTexts) return TEXT_DIRECTION_LTR;
        return static_cast<TextDirection>(typedTexts[typedTextId].textDirection);
    }

    static void registerTypedTextDatabase(const TypedTextData* data, const Font* const* f, const uint16_t n)
    {
        typedTexts = data;
        fonts = f;
        numberOfTypedTexts = n;
    }

    static void registerTexts(const Texts* t)
    {
        if (t) texts = *t;
    }

private:
    TypedTextId typedTextId;
    static const TypedTextData* typedTexts;
    static const Font* const* fonts;
    static uint16_t numberOfTypedTexts;
    static Texts texts;
};

} // namespace touchgfx

#endif // TOUCHGFX_TYPEDTEXT_HPP
