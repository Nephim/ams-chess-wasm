#ifndef TOUCHGFX_UNICODE_HPP
#define TOUCHGFX_UNICODE_HPP

#include <touchgfx/hal/Types.hpp>
#include <stdarg.h>
#include <string>

namespace touchgfx
{

class Unicode
{
public:
    typedef uint16_t UnicodeChar;

    static uint16_t strlen(const UnicodeChar* str);
    static uint16_t strncpy(UnicodeChar* restrictDst, const UnicodeChar* restrictSrc, uint16_t maxchars);
    static uint16_t strncpy(UnicodeChar* restrictDst, const char* restrictSrc, uint16_t maxchars);
    static UnicodeChar* snprintf(UnicodeChar* dst, uint16_t dstSize, const char* format, ...);
    static UnicodeChar* snprintf(UnicodeChar* dst, uint16_t dstSize, const UnicodeChar* format, ...);
    static UnicodeChar* vsnprintf(UnicodeChar* dst, uint16_t dstSize, const char* format, va_list pArg);

    static std::string toUTF8(const UnicodeChar* str, const UnicodeChar* wildcard1 = nullptr, const UnicodeChar* wildcard2 = nullptr);
};

} // namespace touchgfx

#endif // TOUCHGFX_UNICODE_HPP
