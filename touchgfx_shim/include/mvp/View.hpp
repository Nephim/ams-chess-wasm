#ifndef TOUCHGFX_VIEW_HPP
#define TOUCHGFX_VIEW_HPP

#include <touchgfx/containers/Container.hpp>
#include <mvp/Presenter.hpp>

namespace touchgfx
{

class Screen
{
public:
    Screen() : container()
    {
        container.setPosition(0, 0, 480, 272);
    }

    virtual ~Screen() {}

    virtual void setupScreen() {}
    virtual void afterTransition() {}
    virtual void tearDownScreen() {}
    virtual void handleTickEvent() {}
    virtual void handleClickEvent(const ClickEvent& event)
    {
        container.handleClickEvent(event);
    }

    void add(Drawable& d) { container.add(d); }
    void remove(Drawable& d) { container.remove(d); }
    Container& getRootContainer() { return container; }
    const Container& getRootContainer() const { return container; }

protected:
    Container container;
};

template <class T>
class View : public Screen
{
public:
    View() : Screen(), presenter(nullptr) {}
    virtual ~View() {}

    void bind(T& pres) { presenter = &pres; }

protected:
    T* presenter;
};

} // namespace touchgfx

#endif // TOUCHGFX_VIEW_HPP
