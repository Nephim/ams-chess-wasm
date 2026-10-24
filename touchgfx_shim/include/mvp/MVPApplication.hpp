#ifndef TOUCHGFX_MVPAPPLICATION_HPP
#define TOUCHGFX_MVPAPPLICATION_HPP

#include <touchgfx/Application.hpp>
#include <touchgfx/Callback.hpp>
#include <mvp/View.hpp>
#include <mvp/Presenter.hpp>
#include <mvp/MVPHeap.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <new>

namespace touchgfx
{

class MVPApplication : public Application
{
public:
    MVPApplication()
        : Application(),
          currentScreen(nullptr),
          currentPresenter(nullptr),
          currentTransition(nullptr),
          pendingScreenTransitionCallback(nullptr)
    {}

    virtual ~MVPApplication() {}

    virtual void handleTickEvent() override
    {
        evaluatePendingScreenTransition();
        Application::handleTickEvent();
        if (currentScreen)
        {
            currentScreen->handleTickEvent();
        }
    }

    virtual void draw() override
    {
        evaluatePendingScreenTransition();
        if (currentScreen)
        {
            currentScreen->getRootContainer().draw(0, 0, 255);
        }
    }

    void evaluatePendingScreenTransition()
    {
        if (pendingScreenTransitionCallback && pendingScreenTransitionCallback->isValid())
        {
            GenericCallback<>* cb = pendingScreenTransitionCallback;
            pendingScreenTransitionCallback = nullptr;
            cb->execute();
        }
    }

    Screen* getCurrentScreen() const { return currentScreen; }

protected:
    Screen* currentScreen;
    Presenter* currentPresenter;
    Transition* currentTransition;
    GenericCallback<>* pendingScreenTransitionCallback;
};

template <class ScreenType, class PresenterType, class TransType, class ModelType>
void makeTransition(Screen** currentScreen, Presenter** currentPresenter, MVPHeap& heap, Transition** currentTrans, ModelType* model)
{
    if (*currentScreen)
    {
        (*currentScreen)->tearDownScreen();
        (*currentScreen)->~Screen();
        *currentScreen = nullptr;
    }
    if (*currentPresenter)
    {
        (*currentPresenter)->deactivate();
        (*currentPresenter)->~Presenter();
        *currentPresenter = nullptr;
    }
    if (*currentTrans)
    {
        (*currentTrans)->tearDown();
        (*currentTrans)->~Transition();
        *currentTrans = nullptr;
    }

    ScreenType* newScreen = new (heap.screenStorage.allocate<ScreenType>()) ScreenType();
    PresenterType* newPresenter = new (heap.presenterStorage.allocate<PresenterType>()) PresenterType(*newScreen);
    TransType* newTrans = new (heap.transitionStorage.allocate<TransType>()) TransType();

    *currentScreen = newScreen;
    *currentPresenter = newPresenter;
    *currentTrans = newTrans;

    if (model)
    {
        model->bind(newPresenter);
    }
    newPresenter->bind(model);
    newScreen->bind(*newPresenter);

    newPresenter->activate();
    newScreen->setupScreen();
    newTrans->init();
    newScreen->afterTransition();
}

} // namespace touchgfx

#endif // TOUCHGFX_MVPAPPLICATION_HPP
