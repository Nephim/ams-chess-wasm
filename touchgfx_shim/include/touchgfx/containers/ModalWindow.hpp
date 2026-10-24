#ifndef TOUCHGFX_MODALWINDOW_HPP
#define TOUCHGFX_MODALWINDOW_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/Bitmap.hpp>

namespace touchgfx
{

class ModalWindow : public Container
{
public:
    ModalWindow()
        : Container(), backgroundShade(480, 272, Color::getColorFromRGB(0, 0, 0), 150)
    {
        setPosition(0, 0, 480, 272);
        setTouchable(true);
    }

    virtual ~ModalWindow() {}

    void setBackground(BitmapId bmpId)
    {
        Bitmap bmp(bmpId);
        windowBackground.setBitmap(bmp);
        int16_t wx = (480 - bmp.getWidth()) / 2;
        int16_t wy = (272 - bmp.getHeight()) / 2;
        windowContainer.setPosition(wx, wy, bmp.getWidth(), bmp.getHeight());
    }

    void setBackground(BitmapId bmpId, int16_t backgroundX, int16_t backgroundY)
    {
        Bitmap bmp(bmpId);
        windowBackground.setBitmap(bmp);
        windowContainer.setPosition(backgroundX, backgroundY, bmp.getWidth(), bmp.getHeight());
    }

    void setShadeColor(colortype color)
    {
        backgroundShade.setColor(color);
    }

    colortype getShadeColor() const
    {
        return backgroundShade.getColor();
    }

    void setShadeAlpha(uint8_t alpha)
    {
        backgroundShade.setAlpha(alpha);
    }

    uint8_t getShadeAlpha() const
    {
        return backgroundShade.getAlpha();
    }

    void show()
    {
        setVisible(true);
        invalidate();
    }

    void hide()
    {
        setVisible(false);
        invalidate();
    }

    virtual void add(Drawable& d) override
    {
        windowContainer.add(d);
    }

    virtual void remove(Drawable& d) override
    {
        windowContainer.remove(d);
    }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!visible) return;
        int16_t ax = parentAbsX + rect.x;
        int16_t ay = parentAbsY + rect.y;

        backgroundShade.draw(ax, ay, parentAlpha);
        if (windowBackground.getBitmapId() != BITMAP_INVALID)
        {
            SDLBackend::drawBitmap(windowBackground.getBitmapId(),
                                   ax + windowContainer.getX(),
                                   ay + windowContainer.getY(),
                                   windowBackground.getWidth(),
                                   windowBackground.getHeight(),
                                   parentAlpha);
        }
        windowContainer.draw(ax, ay, parentAlpha);
    }

    virtual Drawable* findTouchable(int16_t localX, int16_t localY, int16_t& outRelX, int16_t& outRelY) override
    {
        if (!visible) return nullptr;

        int16_t winX = localX - windowContainer.getX();
        int16_t winY = localY - windowContainer.getY();
        if (winX >= 0 && winX < windowContainer.getWidth() &&
            winY >= 0 && winY < windowContainer.getHeight())
        {
            Drawable* hit = windowContainer.findTouchable(winX, winY, outRelX, outRelY);
            if (hit) return hit;
        }

        // Modal window swallows clicks outside its inner controls when visible
        outRelX = localX;
        outRelY = localY;
        return this;
    }

protected:
    Box backgroundShade;
    Image windowBackground;
    Container windowContainer;
};

} // namespace touchgfx

#endif // TOUCHGFX_MODALWINDOW_HPP
