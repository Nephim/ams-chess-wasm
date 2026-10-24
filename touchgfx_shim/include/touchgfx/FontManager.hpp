#ifndef TOUCHGFX_FONTMANAGER_HPP
#define TOUCHGFX_FONTMANAGER_HPP

#include <touchgfx/Font.hpp>

namespace touchgfx
{

class FontProvider
{
public:
    virtual ~FontProvider() {}
    virtual Font* getFont(FontId fontId) = 0;
};

class FontManager
{
public:
    static void setFontProvider(FontProvider* provider) { fontProvider = provider; }
    static Font* getFont(FontId fontId) { return fontProvider ? fontProvider->getFont(fontId) : nullptr; }

private:
    static FontProvider* fontProvider;
};

} // namespace touchgfx

#endif // TOUCHGFX_FONTMANAGER_HPP
