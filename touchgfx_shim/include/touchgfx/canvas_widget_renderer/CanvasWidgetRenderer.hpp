#ifndef TOUCHGFX_CANVASWIDGETRENDERER_HPP
#define TOUCHGFX_CANVASWIDGETRENDERER_HPP

#include <touchgfx/hal/Types.hpp>

namespace touchgfx
{

class CanvasWidgetRenderer
{
public:
    static void setupBuffer(uint8_t* /*buffer*/, uint32_t /*bufsize*/) {}
    static void resetBuffer() {}
};

} // namespace touchgfx

#endif // TOUCHGFX_CANVASWIDGETRENDERER_HPP
