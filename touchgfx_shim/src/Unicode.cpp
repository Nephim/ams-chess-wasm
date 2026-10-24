#include <touchgfx/Unicode.hpp>
#include <cstdio>
#include <cstring>

namespace touchgfx
{

uint16_t Unicode::strlen(const UnicodeChar* str)
{
    if (!str) return 0;
    uint16_t len = 0;
    while (str[len] != 0)
    {
        len++;
    }
    return len;
}

uint16_t Unicode::strncpy(UnicodeChar* restrictDst, const UnicodeChar* restrictSrc, uint16_t maxchars)
{
    if (!restrictDst || !restrictSrc || maxchars == 0) return 0;
    uint16_t i = 0;
    for (; i < maxchars && restrictSrc[i] != 0; ++i)
    {
        restrictDst[i] = restrictSrc[i];
    }
    if (i < maxchars)
    {
        restrictDst[i] = 0;
    }
    return i;
}

uint16_t Unicode::strncpy(UnicodeChar* restrictDst, const char* restrictSrc, uint16_t maxchars)
{
    if (!restrictDst || !restrictSrc || maxchars == 0) return 0;
    uint16_t i = 0;
    for (; i < maxchars && restrictSrc[i] != 0; ++i)
    {
        restrictDst[i] = static_cast<UnicodeChar>(static_cast<unsigned char>(restrictSrc[i]));
    }
    if (i < maxchars)
    {
        restrictDst[i] = 0;
    }
    return i;
}

Unicode::UnicodeChar* Unicode::vsnprintf(UnicodeChar* dst, uint16_t dstSize, const char* format, va_list pArg)
{
    if (!dst || dstSize == 0) return dst;
    if (!format)
    {
        dst[0] = 0;
        return dst;
    }

    uint16_t outIdx = 0;
    const uint16_t maxOut = dstSize - 1;

    for (size_t i = 0; format[i] != '\0' && outIdx < maxOut; ++i)
    {
        if (format[i] != '%')
        {
            dst[outIdx++] = static_cast<UnicodeChar>(static_cast<unsigned char>(format[i]));
            continue;
        }

        ++i;
        if (format[i] == '\0') break;

        // Parse optional flags/width (e.g. %02d)
        char specBuf[16];
        size_t specIdx = 0;
        specBuf[specIdx++] = '%';
        while (format[i] != '\0' &&
               (format[i] == '-' || format[i] == '+' || format[i] == ' ' || format[i] == '#' ||
                (format[i] >= '0' && format[i] <= '9') || format[i] == '.'))
        {
            if (specIdx < sizeof(specBuf) - 2)
            {
                specBuf[specIdx++] = format[i];
            }
            ++i;
        }

        char conv = format[i];
        if (conv == '\0') break;

        if (conv == '%')
        {
            dst[outIdx++] = static_cast<UnicodeChar>('%');
        }
        else if (conv == 'c')
        {
            int ch = va_arg(pArg, int);
            dst[outIdx++] = static_cast<UnicodeChar>(ch & 0xFFFF);
        }
        else if (conv == 's')
        {
            // In TouchGFX Unicode::snprintf, %s takes const UnicodeChar*
            const UnicodeChar* ustr = va_arg(pArg, const UnicodeChar*);
            if (ustr)
            {
                for (size_t k = 0; ustr[k] != 0 && outIdx < maxOut; ++k)
                {
                    dst[outIdx++] = ustr[k];
                }
            }
        }
        else if (conv == 'd' || conv == 'i')
        {
            specBuf[specIdx++] = conv;
            specBuf[specIdx] = '\0';
            int val = va_arg(pArg, int);
            char numBuf[32];
            std::snprintf(numBuf, sizeof(numBuf), specBuf, val);
            for (size_t k = 0; numBuf[k] != '\0' && outIdx < maxOut; ++k)
            {
                dst[outIdx++] = static_cast<UnicodeChar>(static_cast<unsigned char>(numBuf[k]));
            }
        }
        else if (conv == 'u' || conv == 'x' || conv == 'X')
        {
            specBuf[specIdx++] = conv;
            specBuf[specIdx] = '\0';
            unsigned int val = va_arg(pArg, unsigned int);
            char numBuf[32];
            std::snprintf(numBuf, sizeof(numBuf), specBuf, val);
            for (size_t k = 0; numBuf[k] != '\0' && outIdx < maxOut; ++k)
            {
                dst[outIdx++] = static_cast<UnicodeChar>(static_cast<unsigned char>(numBuf[k]));
            }
        }
        else
        {
            dst[outIdx++] = static_cast<UnicodeChar>(static_cast<unsigned char>(conv));
        }
    }

    dst[outIdx] = 0;
    return dst;
}

Unicode::UnicodeChar* Unicode::snprintf(UnicodeChar* dst, uint16_t dstSize, const char* format, ...)
{
    va_list pArg;
    va_start(pArg, format);
    UnicodeChar* res = vsnprintf(dst, dstSize, format, pArg);
    va_end(pArg);
    return res;
}

Unicode::UnicodeChar* Unicode::snprintf(UnicodeChar* dst, uint16_t dstSize, const UnicodeChar* format, ...)
{
    if (!format)
    {
        if (dst && dstSize > 0) dst[0] = 0;
        return dst;
    }
    std::string asciiFmt;
    for (size_t i = 0; format[i] != 0; ++i)
    {
        asciiFmt.push_back(static_cast<char>(format[i] & 0xFF));
    }
    va_list pArg;
    va_start(pArg, format);
    UnicodeChar* res = vsnprintf(dst, dstSize, asciiFmt.c_str(), pArg);
    va_end(pArg);
    return res;
}

static void appendUnicodeCharUTF8(std::string& out, Unicode::UnicodeChar c)
{
    if (c < 0x80)
    {
        out.push_back(static_cast<char>(c));
    }
    else if (c < 0x800)
    {
        out.push_back(static_cast<char>(0xC0 | ((c >> 6) & 0x1F)));
        out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    }
    else
    {
        out.push_back(static_cast<char>(0xE0 | ((c >> 12) & 0x0F)));
        out.push_back(static_cast<char>(0x80 | ((c >> 6) & 0x3F)));
        out.push_back(static_cast<char>(0x80 | (c & 0x3F)));
    }
}

std::string Unicode::toUTF8(const UnicodeChar* str, const UnicodeChar* wildcard1, const UnicodeChar* wildcard2)
{
    std::string out;
    if (!str) return out;

    int wildcardIndex = 0;
    for (size_t i = 0; str[i] != 0; ++i)
    {
        if (str[i] == 0x0002) // TouchGFX wildcard placeholder
        {
            const UnicodeChar* wc = (wildcardIndex == 0) ? wildcard1 : wildcard2;
            wildcardIndex++;
            if (wc)
            {
                for (size_t j = 0; wc[j] != 0; ++j)
                {
                    appendUnicodeCharUTF8(out, wc[j]);
                }
            }
        }
        else
        {
            appendUnicodeCharUTF8(out, str[i]);
        }
    }
    return out;
}

} // namespace touchgfx
