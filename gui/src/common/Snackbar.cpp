#include <gui/common/Snackbar.hpp>
#include <touchgfx/containers/Container.hpp>

Snackbar::Snackbar(touchgfx::Container* parentContainer, const uint16_t& bitmapId, int x, int y, int fadeInDuration, int visibleDuration, int fadeOutDuration)
    : parentContainer(parentContainer), fadeInDuration(fadeInDuration), visibleDuration(visibleDuration), fadeOutDuration(fadeOutDuration),
    fadeOutAnimationCallback(this, &Snackbar::onFadeOutAnimationEnded)
{
    image.setBitmap(Bitmap(bitmapId));
    image.setXY(x, y);
    image.setAlpha(0);

    parentContainer->add(image);

    image.setFadeAnimationEndedAction(fadeOutAnimationCallback);
    image.startFadeAnimation(255, fadeInDuration, EasingEquations::quintEaseIn);
}

Snackbar::~Snackbar()
{
}

void Snackbar::startFadeOutAnimation()
{
    image.setFadeAnimationDelay(visibleDuration);
    image.startFadeAnimation(0, fadeOutDuration, EasingEquations::quintEaseIn);
}

void Snackbar::onFadeOutAnimationEnded(const FadeAnimator<Image>& animator)
{
    if (image.getAlpha() == 0)
    {
        parentContainer->remove(image);
        delete this;
    }
    else
    {
        startFadeOutAnimation();
    }
}
