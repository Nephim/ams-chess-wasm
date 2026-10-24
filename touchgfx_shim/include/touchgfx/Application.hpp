#ifndef TOUCHGFX_APPLICATION_HPP
#define TOUCHGFX_APPLICATION_HPP

#include <touchgfx/Drawable.hpp>
#include <touchgfx/events/ClickEvent.hpp>
#include <vector>
#include <algorithm>

namespace touchgfx
{

class Screen;

class Application
{
public:
    Application();
    virtual ~Application() {}

    static Application* getInstance() { return instance; }

    virtual void handleTickEvent()
    {
        std::vector<Drawable*> timersCopy = timerWidgets;
        for (Drawable* w : timersCopy)
        {
            if (std::find(timerWidgets.begin(), timerWidgets.end(), w) != timerWidgets.end())
            {
                w->handleTickEvent();
            }
        }
    }

    virtual void handleClickEvent(const ClickEvent& /*event*/) {}
    virtual void draw() {}

    void registerTimerWidget(Drawable* w)
    {
        if (w && std::find(timerWidgets.begin(), timerWidgets.end(), w) == timerWidgets.end())
        {
            timerWidgets.push_back(w);
        }
    }

    void unregisterTimerWidget(const Drawable* w)
    {
        auto it = std::find(timerWidgets.begin(), timerWidgets.end(), w);
        if (it != timerWidgets.end())
        {
            timerWidgets.erase(it);
        }
    }

protected:
    static Application* instance;
    std::vector<Drawable*> timerWidgets;
};

} // namespace touchgfx

#endif // TOUCHGFX_APPLICATION_HPP
