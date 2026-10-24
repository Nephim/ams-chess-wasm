#ifndef TOUCHGFX_TEXTAREAWITHWILDCARD_HPP
#define TOUCHGFX_TEXTAREAWITHWILDCARD_HPP

#include <touchgfx/widgets/TextArea.hpp>

namespace touchgfx
{

class TextAreaWithOneWildcard : public TextArea
{
public:
    TextAreaWithOneWildcard()
        : TextArea(), wildcard(nullptr)
    {}

    virtual ~TextAreaWithOneWildcard() {}

    void setWildcard(const Unicode::UnicodeChar* value)
    {
        wildcard = value;
    }

    const Unicode::UnicodeChar* getWildcard() const
    {
        return wildcard;
    }

    virtual void resizeToCurrentText() override
    {
        int16_t w = 0, h = 0;
        SDLBackend::measureText(typedText, wildcard, nullptr, w, h);
        if (w > 0) rect.width = w;
        if (h > 0) rect.height = h;
    }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!visible || alpha == 0 || parentAlpha == 0 || !typedText.hasValidId()) return;
        uint8_t effAlpha = static_cast<uint8_t>((static_cast<uint16_t>(alpha) * parentAlpha) / 255);
        SDLBackend::drawText(typedText, wildcard, nullptr,
                             parentAbsX + rect.x, parentAbsY + rect.y,
                             rect.width, rect.height, color, effAlpha, false);
    }

protected:
    const Unicode::UnicodeChar* wildcard;
};

class TextAreaWithTwoWildcards : public TextArea
{
public:
    TextAreaWithTwoWildcards()
        : TextArea(), wc1(nullptr), wc2(nullptr)
    {}

    void setWildcard1(const Unicode::UnicodeChar* value) { wc1 = value; }
    void setWildcard2(const Unicode::UnicodeChar* value) { wc2 = value; }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!visible || alpha == 0 || parentAlpha == 0 || !typedText.hasValidId()) return;
        uint8_t effAlpha = static_cast<uint8_t>((static_cast<uint16_t>(alpha) * parentAlpha) / 255);
        SDLBackend::drawText(typedText, wc1, wc2,
                             parentAbsX + rect.x, parentAbsY + rect.y,
                             rect.width, rect.height, color, effAlpha, false);
    }

protected:
    const Unicode::UnicodeChar* wc1;
    const Unicode::UnicodeChar* wc2;
};

} // namespace touchgfx

#endif // TOUCHGFX_TEXTAREAWITHWILDCARD_HPP
