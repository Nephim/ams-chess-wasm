#include <touchgfx/SDLBackend.hpp>
#include <images/BitmapDatabase.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include <tuple>
#include <cstdio>

namespace touchgfx
{

namespace
{

struct BitmapAssetInfo
{
    const char* path;
    uint16_t width;
    uint16_t height;
    SDL_Texture* texture;
};

// Exact 1-to-1 mapping of all 31 BITMAP_*_ID constants in BitmapDatabase.hpp to raw .png assets
static BitmapAssetInfo s_bitmapAssets[31] = {
    { "assets/images/__generated/alternate_theme_images_backgrounds_480x272_gradient_light.png", 480, 272, nullptr }, // 0
    { "assets/images/__generated/alternate_theme_images_containers_small_wide_outlined_light.png", 214, 150, nullptr }, // 1
    { "assets/images/__generated/alternate_theme_images_widgets_button_icon_round_tiny_outline_normal.png", 60, 60, nullptr }, // 2
    { "assets/images/__generated/alternate_theme_images_widgets_button_icon_round_tiny_outline_pressed.png", 60, 60, nullptr }, // 3
    { "assets/images/__generated/alternate_theme_images_widgets_button_regular_height_36_small_round_disabled.png", 138, 36, nullptr }, // 4
    { "assets/images/__generated/alternate_theme_images_widgets_slider_horizontal_thin_filler_small.png", 200, 5, nullptr }, // 5
    { "assets/images/__generated/alternate_theme_images_widgets_slider_horizontal_thin_round_dark.png", 24, 24, nullptr }, // 6
    { "assets/images/__generated/alternate_theme_images_widgets_slider_horizontal_thin_track_small.png", 200, 5, nullptr }, // 7
    { "assets/images/blackbishop.png", 84, 90, nullptr }, // 8
    { "assets/images/blackhorse.png", 77, 76, nullptr }, // 9
    { "assets/images/blackking.png", 78, 85, nullptr }, // 10
    { "assets/images/blackpawn.png", 55, 76, nullptr }, // 11
    { "assets/images/blackqueen.png", 90, 83, nullptr }, // 12
    { "assets/images/blackrook.png", 64, 75, nullptr }, // 13
    { "assets/images/board_brown_sand.png", 1417, 1417, nullptr }, // 14
    { "assets/images/CheckImage.png", 100, 40, nullptr }, // 15
    { "assets/images/CheckmateImage.png", 168, 40, nullptr }, // 16
    { "assets/images/__generated/icon_theme_images_action_done_50_50_E8F6FB.svg.png", 50, 50, nullptr }, // 17
    { "assets/images/__generated/icon_theme_images_content_save_50_50_E8F6FB.svg.png", 50, 50, nullptr }, // 18
    { "assets/images/__generated/icon_theme_images_file_download_50_50_E8E9FA.svg.png", 50, 50, nullptr }, // 19
    { "assets/images/__generated/icon_theme_images_hardware_computer_50_50_E8F6FB.svg.png", 50, 50, nullptr }, // 20
    { "assets/images/__generated/icon_theme_images_maps_celebration_49_48_FFD700.svg.png", 49, 48, nullptr }, // 21
    { "assets/images/__generated/icon_theme_images_navigation_close_50_50_E60000.svg.png", 50, 50, nullptr }, // 22
    { "assets/images/__generated/icon_theme_images_social_people_50_50_E8F6FB.svg.png", 50, 50, nullptr }, // 23
    { "assets/images/MainMenu.png", 300, 200, nullptr }, // 24
    { "assets/images/whitebishop.png", 84, 90, nullptr }, // 25
    { "assets/images/whitehorse.png", 78, 76, nullptr }, // 26
    { "assets/images/whiteking.png", 79, 85, nullptr }, // 27
    { "assets/images/whitepawn.png", 61, 82, nullptr }, // 28
    { "assets/images/whitequeen.png", 89, 83, nullptr }, // 29
    { "assets/images/whiterook.png", 65, 76, nullptr }  // 30
};

static const int kNumBitmaps = sizeof(s_bitmapAssets) / sizeof(s_bitmapAssets[0]);
static const int kHiDpiScale = 3; // 1440x816 physical / 480x272 logical = 3x

static SDL_Renderer* s_renderer = nullptr;
static std::map<int, TTF_Font*> s_ttfFonts;
static bool s_isDirty = true;

struct CachedTextEntry
{
    SDL_Texture* texture;
    int physW;
    int physH;
};

using TextCacheKey = std::tuple<int, std::string, uint32_t>;
static std::map<TextCacheKey, CachedTextEntry> s_textCache;

TTF_Font* getOrOpenFont(int logicalPx)
{
    if (logicalPx <= 0) logicalPx = 12;
    auto it = s_ttfFonts.find(logicalPx);
    if (it != s_ttfFonts.end())
    {
        return it->second;
    }

    int physPx = logicalPx * kHiDpiScale;
    TTF_Font* font = TTF_OpenFont("assets/fonts/verdana.ttf", physPx);
    if (!font)
    {
        std::printf("Failed to load verdana.ttf at %dpx: %s\n", physPx, TTF_GetError());
    }
    s_ttfFonts[logicalPx] = font;
    return font;
}

const CachedTextEntry* getOrCreateTextTexture(int logicalPx, const std::string& utf8, colortype color)
{
    if (utf8.empty() || !s_renderer) return nullptr;

    TextCacheKey key(logicalPx, utf8, color);
    auto it = s_textCache.find(key);
    if (it != s_textCache.end())
    {
        return &it->second;
    }

    TTF_Font* ttfFont = getOrOpenFont(logicalPx);
    if (!ttfFont) return nullptr;

    SDL_Color sdlCol = {
        Color::getRed(color),
        Color::getGreen(color),
        Color::getBlue(color),
        255
    };

    SDL_Surface* surf = TTF_RenderUTF8_Blended(ttfFont, utf8.c_str(), sdlCol);
    if (!surf) return nullptr;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(s_renderer, surf);
    int pw = surf->w;
    int ph = surf->h;
    SDL_FreeSurface(surf);

    if (!tex) return nullptr;

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    CachedTextEntry entry{ tex, pw, ph };
    auto inserted = s_textCache.emplace(std::move(key), entry);
    return &inserted.first->second;
}

} // anonymous namespace

bool SDLBackend::init(SDL_Renderer* renderer)
{
    s_renderer = renderer;
    SDL_SetRenderDrawBlendMode(s_renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < kNumBitmaps; ++i)
    {
        SDL_Texture* tex = IMG_LoadTexture(s_renderer, s_bitmapAssets[i].path);
        if (!tex)
        {
            std::printf("Warning: Failed to load texture %s: %s\n", s_bitmapAssets[i].path, IMG_GetError());
        }
        else
        {
            SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        }
        s_bitmapAssets[i].texture = tex;
    }

    // Pre-open standard typography sizes from ApplicationFontProvider.hpp (10px, 12px, 20px, 40px) at 3x DPI
    getOrOpenFont(10);
    getOrOpenFont(12);
    getOrOpenFont(20);
    getOrOpenFont(40);

    s_isDirty = true;
    return true;
}

void SDLBackend::shutdown()
{
    for (auto& kv : s_textCache)
    {
        if (kv.second.texture)
        {
            SDL_DestroyTexture(kv.second.texture);
        }
    }
    s_textCache.clear();

    for (auto& kv : s_ttfFonts)
    {
        if (kv.second)
        {
            TTF_CloseFont(kv.second);
        }
    }
    s_ttfFonts.clear();

    for (int i = 0; i < kNumBitmaps; ++i)
    {
        if (s_bitmapAssets[i].texture)
        {
            SDL_DestroyTexture(s_bitmapAssets[i].texture);
            s_bitmapAssets[i].texture = nullptr;
        }
    }

    s_renderer = nullptr;
}

SDL_Renderer* SDLBackend::getRenderer()
{
    return s_renderer;
}

uint16_t SDLBackend::getBitmapWidth(BitmapId id)
{
    if (id < kNumBitmaps)
    {
        return s_bitmapAssets[id].width;
    }
    return 0;
}

uint16_t SDLBackend::getBitmapHeight(BitmapId id)
{
    if (id < kNumBitmaps)
    {
        return s_bitmapAssets[id].height;
    }
    return 0;
}

void SDLBackend::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, colortype color, uint8_t alpha)
{
    if (!s_renderer || w <= 0 || h <= 0 || alpha == 0) return;
    SDL_SetRenderDrawColor(s_renderer,
                           Color::getRed(color),
                           Color::getGreen(color),
                           Color::getBlue(color),
                           alpha);
    SDL_Rect r = { x, y, w, h };
    SDL_RenderFillRect(s_renderer, &r);
}

void SDLBackend::drawBitmap(BitmapId id, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t alpha)
{
    if (!s_renderer || id >= kNumBitmaps || alpha == 0) return;
    SDL_Texture* tex = s_bitmapAssets[id].texture;
    if (!tex) return;

    int16_t drawW = (w > 0) ? w : static_cast<int16_t>(s_bitmapAssets[id].width);
    int16_t drawH = (h > 0) ? h : static_cast<int16_t>(s_bitmapAssets[id].height);
    if (drawW <= 0 || drawH <= 0) return;

    SDL_SetTextureAlphaMod(tex, alpha);
    SDL_Rect dst = { x, y, drawW, drawH };
    SDL_RenderCopy(s_renderer, tex, nullptr, &dst);
}

void SDLBackend::drawBitmapClipped(BitmapId id, int16_t x, int16_t y, int16_t clipW, int16_t clipH, uint8_t alpha)
{
    if (!s_renderer || id >= kNumBitmaps || clipW <= 0 || clipH <= 0 || alpha == 0) return;
    SDL_Texture* tex = s_bitmapAssets[id].texture;
    if (!tex) return;

    int16_t logW = static_cast<int16_t>(s_bitmapAssets[id].width);
    int16_t logH = static_cast<int16_t>(s_bitmapAssets[id].height);
    if (logW <= 0 || logH <= 0) return;

    int16_t w = std::min<int16_t>(clipW, logW);
    int16_t h = std::min<int16_t>(clipH, logH);

    int texW = logW, texH = logH;
    SDL_QueryTexture(tex, nullptr, nullptr, &texW, &texH);

    SDL_SetTextureAlphaMod(tex, alpha);
    SDL_Rect src = {
        0,
        0,
        static_cast<int>((static_cast<int32_t>(w) * texW) / logW),
        static_cast<int>((static_cast<int32_t>(h) * texH) / logH)
    };
    SDL_Rect dst = { x, y, w, h };
    SDL_RenderCopy(s_renderer, tex, &src, &dst);
}

void SDLBackend::measureText(const TypedText& typedText,
                             const Unicode::UnicodeChar* wildcard1,
                             const Unicode::UnicodeChar* wildcard2,
                             int16_t& outW, int16_t& outH)
{
    outW = 0;
    outH = 0;
    if (!typedText.hasValidId()) return;

    const Unicode::UnicodeChar* raw = typedText.getText();
    if (!raw) return;

    std::string utf8 = Unicode::toUTF8(raw, wildcard1, wildcard2);
    if (utf8.empty()) return;

    const Font* font = typedText.getFont();
    int logicalPx = font ? font->getFontHeight() : 12;

    TTF_Font* ttfFont = getOrOpenFont(logicalPx);
    if (!ttfFont) return;

    int pw = 0, ph = 0;
    if (TTF_SizeUTF8(ttfFont, utf8.c_str(), &pw, &ph) == 0)
    {
        outW = static_cast<int16_t>((pw + kHiDpiScale - 1) / kHiDpiScale);
        outH = static_cast<int16_t>((ph + kHiDpiScale - 1) / kHiDpiScale);
    }
}

void SDLBackend::drawText(const TypedText& typedText,
                          const Unicode::UnicodeChar* wildcard1,
                          const Unicode::UnicodeChar* wildcard2,
                          int16_t x, int16_t y, int16_t w, int16_t h,
                          colortype color, uint8_t alpha,
                          bool centerVertically)
{
    if (!s_renderer || !typedText.hasValidId() || alpha == 0) return;

    const Unicode::UnicodeChar* raw = typedText.getText();
    if (!raw) return;

    std::string utf8 = Unicode::toUTF8(raw, wildcard1, wildcard2);
    if (utf8.empty()) return;

    const Font* font = typedText.getFont();
    int logicalPx = font ? font->getFontHeight() : 12;

    const CachedTextEntry* cached = getOrCreateTextTexture(logicalPx, utf8, color);
    if (!cached || !cached->texture) return;

    float drawW = static_cast<float>(cached->physW) / static_cast<float>(kHiDpiScale);
    float drawH = static_cast<float>(cached->physH) / static_cast<float>(kHiDpiScale);

    float drawX = static_cast<float>(x);
    float drawY = static_cast<float>(y);

    Alignment align = typedText.getAlignment();
    if (w > 0)
    {
        if (align == CENTER)
        {
            drawX = static_cast<float>(x) + (static_cast<float>(w) - drawW) * 0.5f;
        }
        else if (align == RIGHT)
        {
            drawX = static_cast<float>(x) + static_cast<float>(w) - drawW;
        }
    }

    if (centerVertically && h > 0)
    {
        drawY = static_cast<float>(y) + (static_cast<float>(h) - drawH) * 0.5f;
    }

    SDL_SetTextureAlphaMod(cached->texture, alpha);
    SDL_FRect dst = { drawX, drawY, drawW, drawH };
    SDL_RenderCopyF(s_renderer, cached->texture, nullptr, &dst);
}

void SDLBackend::markDirty()
{
    s_isDirty = true;
}

bool SDLBackend::consumeDirty()
{
    bool wasDirty = s_isDirty;
    s_isDirty = false;
    return wasDirty;
}

} // namespace touchgfx
