#ifndef TOUCHGFX_TEXTPROVIDER_HPP
#define TOUCHGFX_TEXTPROVIDER_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Unicode.hpp>

namespace touchgfx
{

class TextProvider
{
public:
    TextProvider() : fontGsubTable(nullptr) {}

    Unicode::UnicodeChar getNextLigature(TextDirection direction);
    void initializeInternal();

private:
    struct CircularBuffer
    {
        Unicode::UnicodeChar buf[4] = {0, 0, 0, 0};
        void replaceAt0(Unicode::UnicodeChar c) { buf[0] = c; }
        void replaceAt1(Unicode::UnicodeChar c) { buf[1] = c; }
        Unicode::UnicodeChar peekChar(uint16_t offset = 0) const { return buf[offset & 3]; }
    };

    CircularBuffer nextCharacters;
    const uint16_t* fontGsubTable;

    Unicode::UnicodeChar unicodeConverter(TextDirection /*direction*/) { return 0; }
    void substituteGlyphs() {}
    Unicode::UnicodeChar getNextChar() { return nextCharacters.peekChar(0); }
    void fillInputBuffer() {}
    void unicodeConverterInit() {}
};

} // namespace touchgfx

#endif // TOUCHGFX_TEXTPROVIDER_HPP
