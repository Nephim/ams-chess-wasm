#ifndef TOUCHGFX_CLICKLISTENER_HPP
#define TOUCHGFX_CLICKLISTENER_HPP

#include <touchgfx/Callback.hpp>
#include <touchgfx/events/ClickEvent.hpp>

namespace touchgfx
{

template <class T>
class ClickListener : public T
{
public:
    ClickListener()
        : T(), clickAction(nullptr)
    {
        this->setTouchable(true);
    }

    virtual void handleClickEvent(const ClickEvent& event) override
    {
        T::handleClickEvent(event);
        if (clickAction && clickAction->isValid())
        {
            clickAction->execute(*this, event);
        }
    }

    void setClickAction(GenericCallback<const T&, const ClickEvent&>& callback)
    {
        clickAction = &callback;
        this->setTouchable(true);
    }

protected:
    GenericCallback<const T&, const ClickEvent&>* clickAction;
};

} // namespace touchgfx

#endif // TOUCHGFX_CLICKLISTENER_HPP
