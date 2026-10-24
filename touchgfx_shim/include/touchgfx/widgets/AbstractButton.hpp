#ifndef TOUCHGFX_ABSTRACTBUTTON_HPP
#define TOUCHGFX_ABSTRACTBUTTON_HPP

#include <touchgfx/Drawable.hpp>
#include <touchgfx/Callback.hpp>

namespace touchgfx
{

class AbstractButton : public Drawable
{
public:
    AbstractButton()
        : Drawable(), action(nullptr), pressed(false), alpha(255)
    {
        setTouchable(true);
    }

    virtual ~AbstractButton() {}

    void setAction(GenericCallback<const AbstractButton&>& callback)
    {
        action = &callback;
    }

    bool getPressedState() const { return pressed; }
    void setAlpha(uint8_t a) { alpha = a; }
    uint8_t getAlpha() const { return alpha; }

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

protected:
    GenericCallback<const AbstractButton&>* action;
    bool pressed;
    uint8_t alpha;
};

} // namespace touchgfx

#endif // TOUCHGFX_ABSTRACTBUTTON_HPP
