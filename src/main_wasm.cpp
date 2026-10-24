#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <touchgfx/SDLBackend.hpp>
#include <touchgfx/FontManager.hpp>
#include <touchgfx/Texts.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include <gui/common/FrontendHeap.hpp>

#include <cstdio>

namespace
{

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
ApplicationFontProvider g_fontProvider;

touchgfx::Drawable* g_capturedWidget = nullptr;
int16_t g_lastRelX = 0;
int16_t g_lastRelY = 0;
uint32_t g_lastTickMs = 0;

void handlePointerDown(int16_t lx, int16_t ly)
{
    touchgfx::MVPApplication* app = &FrontendHeap::getInstance().app;
    if (!app || !app->getCurrentScreen()) return;

    int16_t relX = 0, relY = 0;
    touchgfx::Drawable* target = app->getCurrentScreen()->getRootContainer().findTouchable(lx, ly, relX, relY);
    g_capturedWidget = target;
    g_lastRelX = relX;
    g_lastRelY = relY;

    if (target)
    {
        touchgfx::ClickEvent ev(touchgfx::ClickEvent::PRESSED, relX, relY);
        target->handleClickEvent(ev);
    }
}

void handlePointerMove(int16_t lx, int16_t ly)
{
    if (!g_capturedWidget) return;

    touchgfx::Rect absRect = g_capturedWidget->getAbsoluteRect();
    int16_t relX = lx - absRect.x;
    int16_t relY = ly - absRect.y;

    if (relX != g_lastRelX || relY != g_lastRelY)
    {
        g_capturedWidget->handleDragEvent(g_lastRelX, g_lastRelY, relX, relY);
        g_lastRelX = relX;
        g_lastRelY = relY;
    }
}

void handlePointerUp(int16_t lx, int16_t ly)
{
    if (!g_capturedWidget) return;

    touchgfx::Drawable* target = g_capturedWidget;
    g_capturedWidget = nullptr;

    touchgfx::Rect absRect = target->getAbsoluteRect();
    int16_t relX = lx - absRect.x;
    int16_t relY = ly - absRect.y;

    bool inside = (relX >= 0 && relX < absRect.width && relY >= 0 && relY < absRect.height);
    touchgfx::ClickEvent ev(
        inside ? touchgfx::ClickEvent::RELEASED : touchgfx::ClickEvent::CANCEL,
        relX, relY);
    target->handleClickEvent(ev);
}

void mainLoopIteration()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                handlePointerDown(static_cast<int16_t>(event.button.x),
                                  static_cast<int16_t>(event.button.y));
            }
            break;

        case SDL_MOUSEMOTION:
            if (event.motion.state & SDL_BUTTON_LMASK)
            {
                handlePointerMove(static_cast<int16_t>(event.motion.x),
                                  static_cast<int16_t>(event.motion.y));
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                handlePointerUp(static_cast<int16_t>(event.button.x),
                                static_cast<int16_t>(event.button.y));
            }
            break;

        default:
            break;
        }
    }

    // Drive TouchGFX tick at ~60Hz
    uint32_t nowMs = SDL_GetTicks();
    if (g_lastTickMs == 0)
    {
        g_lastTickMs = nowMs;
    }
    uint32_t elapsed = nowMs - g_lastTickMs;
    if (elapsed >= 16)
    {
        int ticks = static_cast<int>(elapsed / 16);
        if (ticks > 4) ticks = 4;
        for (int i = 0; i < ticks; ++i)
        {
            FrontendHeap::getInstance().app.handleTickEvent();
        }
        g_lastTickMs = nowMs - (elapsed % 16);
    }

    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
    SDL_RenderClear(g_renderer);

    FrontendHeap::getInstance().app.draw();

    SDL_RenderPresent(g_renderer);
}

} // anonymous namespace

int main(int /*argc*/, char* /*argv*/[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        std::printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0)
    {
        std::printf("IMG_Init error: %s\n", IMG_GetError());
    }

    if (TTF_Init() != 0)
    {
        std::printf("TTF_Init error: %s\n", TTF_GetError());
    }

    // High-DPI SDL2 Logical Renderer setup as specified in AGENTS.md:
    // 3x native resolution (1440x816) with SDL_WINDOW_ALLOW_HIGHDPI and logical size 480x272
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow(
        "TouchGFX Chess - High-DPI WebAssembly",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1440,
        816,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

    if (!g_window)
    {
        std::printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        return 1;
    }

    g_renderer = SDL_CreateRenderer(
        g_window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!g_renderer)
    {
        g_renderer = SDL_CreateRenderer(g_window, -1, 0);
    }

    SDL_RenderSetLogicalSize(g_renderer, 480, 272);

    touchgfx::SDLBackend::init(g_renderer);
    touchgfx::FontManager::setFontProvider(&g_fontProvider);
    touchgfx::Texts::setLanguage(GB);

    // Initialize FrontendHeap and transition immediately to ChessGameScreen
    FrontendHeap& heap = FrontendHeap::getInstance();
    heap.app.evaluatePendingScreenTransition();

    g_lastTickMs = SDL_GetTicks();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoopIteration, 0, 1);
#else
    bool running = true;
    while (running)
    {
        mainLoopIteration();
        SDL_Delay(16);
    }
#endif

    touchgfx::SDLBackend::shutdown();
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
