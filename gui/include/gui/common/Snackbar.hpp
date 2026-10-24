#ifndef SNACKBAR_HPP
#define SNACKBAR_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/EasingEquations.hpp>
#include <touchgfx/containers/Container.hpp>

using namespace touchgfx;

class Snackbar
{
public:
    Snackbar(touchgfx::Container* parentContainer, const uint16_t& bitmapId, int x, int y, int fadeInDuration = 20, int visibleDuration = 50, int fadeOutDuration = 30);
    virtual ~Snackbar();

protected:
    Container* parentContainer;
    FadeAnimator<Image> image;
    int fadeInDuration;
    int visibleDuration;
    int fadeOutDuration;

    Callback<Snackbar, const FadeAnimator<Image>&> fadeOutAnimationCallback;

    void startFadeOutAnimation();
    void onFadeOutAnimationEnded(const FadeAnimator<Image>& animator);
};

#endif /* SNACKBAR_HPP */
