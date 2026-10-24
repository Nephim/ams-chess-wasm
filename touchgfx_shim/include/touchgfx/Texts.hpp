#ifndef TOUCHGFX_TEXTS_HPP
#define TOUCHGFX_TEXTS_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Unicode.hpp>

namespace touchgfx
{

class Texts
{
public:
    static void setLanguage(LanguageId id);
    static LanguageId getLanguage() { return currentLanguage; }
    static void setTranslation(LanguageId id, const void* translation);
    const Unicode::UnicodeChar* getText(TypedTextId id) const;

private:
    static LanguageId currentLanguage;
};

} // namespace touchgfx

#endif // TOUCHGFX_TEXTS_HPP
