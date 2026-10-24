#ifndef TOUCHGFX_HAL_HPP
#define TOUCHGFX_HAL_HPP

#include <touchgfx/hal/Types.hpp>
#include <platform/driver/lcd/LCD24bpp.hpp>

namespace touchgfx
{

class HAL
{
public:
    static HAL* getInstance();
    static LCD& lcd();

    void setDisplayOrientation(DisplayOrientation orientation) { displayOrientation = orientation; }
    DisplayOrientation getDisplayOrientation() const { return displayOrientation; }

private:
    DisplayOrientation displayOrientation = ORIENTATION_LANDSCAPE;
};

} // namespace touchgfx

#endif // TOUCHGFX_HAL_HPP
