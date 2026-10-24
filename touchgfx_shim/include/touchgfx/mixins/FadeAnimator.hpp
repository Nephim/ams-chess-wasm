#ifndef TOUCHGFX_FADEANIMATOR_HPP
#define TOUCHGFX_FADEANIMATOR_HPP

#include <touchgfx/Application.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <algorithm>

namespace touchgfx
{

template <class T>
class FadeAnimator : public T
{
public:
    FadeAnimator()
        : T(),
          fadeAnimationRunning(false),
          fadeAnimationCounter(0),
          fadeAnimationDelay(0),
          fadeAnimationDuration(0),
          fadeAnimationStartAlpha(0),
          fadeAnimationEndAlpha(0),
          fadeAnimationAlphaEquation(&EasingEquations::linearEaseNone),
          fadeAnimationEndedCallback(nullptr)
    {}

    virtual ~FadeAnimator()
    {
        if (Application::getInstance())
        {
            Application::getInstance()->unregisterTimerWidget(this);
        }
    }

    void setFadeAnimationEndedAction(GenericCallback<const FadeAnimator<T>&>& callback)
    {
        fadeAnimationEndedCallback = &callback;
    }

    void clearFadeAnimationEndedAction()
    {
        fadeAnimationEndedCallback = nullptr;
    }

    void setFadeAnimationDelay(uint16_t delay)
    {
        fadeAnimationDelay = delay;
    }

    uint16_t getFadeAnimationDelay() const
    {
        return fadeAnimationDelay;
    }

    bool isFadeAnimationRunning() const
    {
        return fadeAnimationRunning;
    }

    void startFadeAnimation(uint8_t endAlpha, uint16_t duration, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone)
    {
        if (Application::getInstance())
        {
            Application::getInstance()->registerTimerWidget(this);
        }
        fadeAnimationCounter = 0;
        fadeAnimationStartAlpha = this->getAlpha();
        fadeAnimationEndAlpha = endAlpha;
        fadeAnimationDuration = duration;
        fadeAnimationAlphaEquation = alphaProgressionEquation;
        fadeAnimationRunning = true;

        if (fadeAnimationDelay == 0 && fadeAnimationDuration == 0)
        {
            nextFadeAnimationStep();
        }
    }

    void cancelFadeAnimation()
    {
        if (Application::getInstance())
        {
            Application::getInstance()->unregisterTimerWidget(this);
        }
        fadeAnimationRunning = false;
    }

    virtual void handleTickEvent() override
    {
        T::handleTickEvent();
        nextFadeAnimationStep();
    }

protected:
    void nextFadeAnimationStep()
    {
        if (!fadeAnimationRunning) return;

        if (fadeAnimationDelay > 0)
        {
            fadeAnimationDelay--;
            return;
        }

        fadeAnimationCounter++;
        if (fadeAnimationCounter >= fadeAnimationDuration)
        {
            this->setAlpha(fadeAnimationEndAlpha);
            this->invalidate();
            fadeAnimationRunning = false;
            if (Application::getInstance())
            {
                Application::getInstance()->unregisterTimerWidget(this);
            }
            if (fadeAnimationEndedCallback && fadeAnimationEndedCallback->isValid())
            {
                fadeAnimationEndedCallback->execute(*this);
            }
        }
        else
        {
            int16_t delta = static_cast<int16_t>(fadeAnimationEndAlpha) - static_cast<int16_t>(fadeAnimationStartAlpha);
            int16_t newAlpha = fadeAnimationAlphaEquation
                ? fadeAnimationAlphaEquation(fadeAnimationCounter, fadeAnimationStartAlpha, delta, fadeAnimationDuration)
                : static_cast<int16_t>(fadeAnimationStartAlpha + (delta * fadeAnimationCounter) / fadeAnimationDuration);
            newAlpha = std::max<int16_t>(0, std::min<int16_t>(255, newAlpha));
            this->setAlpha(static_cast<uint8_t>(newAlpha));
            this->invalidate();
        }
    }

    bool fadeAnimationRunning;
    uint16_t fadeAnimationCounter;
    uint16_t fadeAnimationDelay;
    uint16_t fadeAnimationDuration;
    uint8_t fadeAnimationStartAlpha;
    uint8_t fadeAnimationEndAlpha;
    EasingEquation fadeAnimationAlphaEquation;
    GenericCallback<const FadeAnimator<T>&>* fadeAnimationEndedCallback;
};

} // namespace touchgfx

#endif // TOUCHGFX_FADEANIMATOR_HPP
