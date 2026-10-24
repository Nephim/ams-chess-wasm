#ifndef TOUCHGFX_SDLBACKEND_HPP
#define TOUCHGFX_SDLBACKEND_HPP

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/Unicode.hpp>

struct SDL_Window;
struct SDL_Renderer;

namespace touchgfx
{

class SDLBackend
{
public:
    static bool init(SDL_Renderer* renderer);
    static void shutdown();

    static SDL_Renderer* getRenderer();

    static uint16_t getBitmapWidth(BitmapId id);
    static uint16_t getBitmapHeight(BitmapId id);

    static void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, colortype color, uint8_t alpha = 255);
    static void drawBitmap(BitmapId id, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t alpha = 255);
    static void drawBitmapClipped(BitmapId id, int16_t x, int16_t y, int16_t clipW, int16_t clipH, uint8_t alpha = 255);

    static void measureText(const TypedText& typedText,
                            const Unicode::UnicodeChar* wildcard1,
                            const Unicode::UnicodeChar* wildcard2,
                            int16_t& outW, int16_t& outH);

    static void drawText(const TypedText& typedText,
                         const Unicode::UnicodeChar* wildcard1,
                         const Unicode::UnicodeChar* wildcard2,
                         int16_t x, int16_t y, int16_t w, int16_t h,
                         colortype color, uint8_t alpha = 255,
                         bool centerVertically = false);

    static void markDirty();
    static bool consumeDirty();
};

} // namespace touchgfx

#endif // TOUCHGFX_SDLBACKEND_HPP
