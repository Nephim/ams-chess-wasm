#ifndef TOUCHGFX_BUTTONS_HPP
#define TOUCHGFX_BUTTONS_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/AbstractButton.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/SDLBackend.hpp>

namespace touchgfx
{

class AbstractButtonContainer : public Container
{
public:
    AbstractButtonContainer()
        : Container(), pressed(false), alpha(255), action(nullptr)
    {
        setTouchable(true);
    }

    void setAction(GenericCallback<const AbstractButtonContainer&>& callback)
    {
        action = &callback;
    }

    bool getPressedState() const { return pressed; }
    void setPressedState(bool isPressed) { pressed = isPressed; }

    void setAlpha(uint8_t a) { alpha = a; }
    uint8_t getAlpha() const { return alpha; }

protected:
    bool pressed;
    uint8_t alpha;
    GenericCallback<const AbstractButtonContainer&>* action;
};

class ClickButtonTrigger : public AbstractButtonContainer
{
public:
    ClickButtonTrigger() : AbstractButtonContainer() {}

    virtual void handleClickEvent(const ClickEvent& event) override
    {
        if (event.getType() == ClickEvent::PRESSED)
        {
            pressed = true;
            invalidate();
        }
        else if (event.getType() == ClickEvent::RELEASED)
        {
            bool wasPressed = pressed;
            pressed = false;
            invalidate();
            if (wasPressed && action && action->isValid())
            {
                action->execute(*this);
            }
        }
        else if (event.getType() == ClickEvent::CANCEL)
        {
            pressed = false;
            invalidate();
        }
    }

    virtual Drawable* findTouchable(int16_t localX, int16_t localY, int16_t& outRelX, int16_t& outRelY) override
    {
        if (!visible) return nullptr;
        if (touchable && localX >= 0 && localX < rect.width && localY >= 0 && localY < rect.height)
        {
            outRelX = localX;
            outRelY = localY;
            return this;
        }
        return nullptr;
    }
};

template <class T>
class BoxWithBorderButtonStyle : public T
{
public:
    BoxWithBorderButtonStyle()
        : T(), boxX(0), boxY(0), boxW(0), boxH(0),
          borderSize(0),
          colorReleased(0), colorPressed(0),
          borderReleased(0), borderPressed(0)
    {}

    void setBoxWithBorderPosition(int16_t x, int16_t y, int16_t width, int16_t height)
    {
        boxX = x;
        boxY = y;
        boxW = width;
        boxH = height;
    }

    void setBorderSize(uint8_t size)
    {
        borderSize = size;
    }

    void setBoxWithBorderColors(const colortype colorRel, const colortype colorPress,
                                const colortype borderColorRel, const colortype borderColorPress)
    {
        colorReleased = colorRel;
        colorPressed = colorPress;
        borderReleased = borderColorRel;
        borderPressed = borderColorPress;
    }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!this->visible || this->alpha == 0 || parentAlpha == 0) return;
        uint8_t effAlpha = static_cast<uint8_t>((static_cast<uint16_t>(this->alpha) * parentAlpha) / 255);
        int16_t ax = parentAbsX + this->rect.x + boxX;
        int16_t ay = parentAbsY + this->rect.y + boxY;

        colortype fillCol = this->pressed ? colorPressed : colorReleased;
        colortype bordCol = this->pressed ? borderPressed : borderReleased;

        if (borderSize > 0)
        {
            SDLBackend::fillRect(ax, ay, boxW, boxH, bordCol, effAlpha);
            int16_t innerW = boxW - 2 * borderSize;
            int16_t innerH = boxH - 2 * borderSize;
            if (innerW > 0 && innerH > 0)
            {
                SDLBackend::fillRect(ax + borderSize, ay + borderSize, innerW, innerH, fillCol, effAlpha);
            }
        }
        else
        {
            SDLBackend::fillRect(ax, ay, boxW, boxH, fillCol, effAlpha);
        }

        T::draw(parentAbsX, parentAbsY, parentAlpha);
    }

protected:
    int16_t boxX, boxY, boxW, boxH;
    uint8_t borderSize;
    colortype colorReleased, colorPressed;
    colortype borderReleased, borderPressed;
};

template <class T>
class IconButtonStyle : public T
{
public:
    IconButtonStyle()
        : T(), iconX(0), iconY(0)
    {}

    void setIconBitmaps(const Bitmap& newIconReleased, const Bitmap& newIconPressed)
    {
        iconReleased = newIconReleased;
        iconPressed = newIconPressed;
    }

    void setIconXY(int16_t x, int16_t y)
    {
        iconX = x;
        iconY = y;
    }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!this->visible || this->alpha == 0 || parentAlpha == 0) return;
        T::draw(parentAbsX, parentAbsY, parentAlpha);

        uint8_t effAlpha = static_cast<uint8_t>((static_cast<uint16_t>(this->alpha) * parentAlpha) / 255);
        const Bitmap& ic = this->pressed ? iconPressed : iconReleased;
        if (ic.getId() != BITMAP_INVALID)
        {
            SDLBackend::drawBitmap(ic.getId(),
                                   parentAbsX + this->rect.x + iconX,
                                   parentAbsY + this->rect.y + iconY,
                                   ic.getWidth(), ic.getHeight(), effAlpha);
        }
    }

protected:
    Bitmap iconReleased;
    Bitmap iconPressed;
    int16_t iconX;
    int16_t iconY;
};

template <class T>
class TextButtonStyle : public T
{
public:
    TextButtonStyle()
        : T(), textX(0), textY(0), textW(0), textH(0),
          textColorReleased(0), textColorPressed(0)
    {}

    void setText(TypedText t)
    {
        typedText = t;
    }

    void setTextPosition(int16_t x, int16_t y, int16_t width, int16_t height)
    {
        textX = x;
        textY = y;
        textW = width;
        textH = height;
    }

    void setTextColors(colortype newTextColorReleased, colortype newTextColorPressed)
    {
        textColorReleased = newTextColorReleased;
        textColorPressed = newTextColorPressed;
    }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!this->visible || this->alpha == 0 || parentAlpha == 0) return;
        T::draw(parentAbsX, parentAbsY, parentAlpha);

        if (typedText.hasValidId())
        {
            uint8_t effAlpha = static_cast<uint8_t>((static_cast<uint16_t>(this->alpha) * parentAlpha) / 255);
            colortype col = this->pressed ? textColorPressed : textColorReleased;
            SDLBackend::drawText(typedText, nullptr, nullptr,
                                 parentAbsX + this->rect.x + textX,
                                 parentAbsY + this->rect.y + textY,
                                 textW, textH, col, effAlpha, false);
        }
    }

protected:
    TypedText typedText;
    int16_t textX, textY, textW, textH;
    colortype textColorReleased, textColorPressed;
};

} // namespace touchgfx

#endif // TOUCHGFX_BUTTONS_HPP
