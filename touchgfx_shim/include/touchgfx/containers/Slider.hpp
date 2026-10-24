#ifndef TOUCHGFX_SLIDER_HPP
#define TOUCHGFX_SLIDER_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/SDLBackend.hpp>
#include <algorithm>

namespace touchgfx
{

class Slider : public Container
{
public:
    Slider()
        : Container(),
          currentValue(0),
          valueRangeMin(0),
          valueRangeMax(100),
          backgroundX(0),
          backgroundY(0),
          fillerX(0),
          fillerY(0),
          indicatorMinPosition(0),
          indicatorMaxPosition(100),
          startValueCallback(nullptr),
          stopValueCallback(nullptr),
          newValueCallback(nullptr)
    {
        setTouchable(true);
    }

    virtual ~Slider() {}

    void setBitmaps(const Bitmap& sliderBackground, const Bitmap& sliderBackgroundSelected, const Bitmap& indicator)
    {
        backgroundBmp = sliderBackground;
        fillerBmp = sliderBackgroundSelected;
        indicatorBmp = indicator;
        updateDimensions();
    }

    void setupHorizontalSlider(uint16_t bgX, uint16_t bgY, uint16_t fillX, uint16_t fillY, uint16_t indMaxPos)
    {
        backgroundX = bgX;
        backgroundY = bgY;
        fillerX = fillX ? fillX : bgX;
        fillerY = fillY ? fillY : bgY;
        indicatorMinPosition = 0;
        indicatorMaxPosition = indMaxPos;
        updateDimensions();
    }

    void setValueRange(int minValue, int maxValue, int newValue)
    {
        valueRangeMin = minValue;
        valueRangeMax = maxValue;
        setValue(newValue);
    }

    void setValueRange(int minValue, int maxValue)
    {
        valueRangeMin = minValue;
        valueRangeMax = maxValue;
        setValue(currentValue);
    }

    void setValue(int value)
    {
        currentValue = std::max(valueRangeMin, std::min(valueRangeMax, value));
        invalidate();
    }

    int getValue() const { return currentValue; }

    void setNewValueCallback(GenericCallback<const Slider&, int>& callback)
    {
        newValueCallback = &callback;
    }

    void setStartValueCallback(GenericCallback<const Slider&, int>& callback)
    {
        startValueCallback = &callback;
    }

    void setStopValueCallback(GenericCallback<const Slider&, int>& callback)
    {
        stopValueCallback = &callback;
    }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!visible || parentAlpha == 0) return;
        int16_t ax = parentAbsX + rect.x;
        int16_t ay = parentAbsY + rect.y;

        if (backgroundBmp.getId() != BITMAP_INVALID)
        {
            SDLBackend::drawBitmap(backgroundBmp.getId(),
                                   ax + backgroundX, ay + backgroundY,
                                   backgroundBmp.getWidth(), backgroundBmp.getHeight(),
                                   parentAlpha);
        }

        int16_t indX = valueToPosition(currentValue);
        int16_t indRadius = indicatorBmp.getWidth() / 2;

        if (fillerBmp.getId() != BITMAP_INVALID)
        {
            int16_t fillW = (indX + indRadius) - backgroundX;
            if (fillW > 0)
            {
                SDLBackend::drawBitmapClipped(fillerBmp.getId(),
                                              ax + backgroundX, ay + backgroundY,
                                              fillW, fillerBmp.getHeight(),
                                              parentAlpha);
            }
        }

        if (indicatorBmp.getId() != BITMAP_INVALID)
        {
            SDLBackend::drawBitmap(indicatorBmp.getId(),
                                   ax + indX, ay,
                                   indicatorBmp.getWidth(), indicatorBmp.getHeight(),
                                   parentAlpha);
        }
    }

    virtual void handleClickEvent(const ClickEvent& event) override
    {
        if (event.getType() == ClickEvent::PRESSED || event.getType() == ClickEvent::RELEASED)
        {
            updateValueFromX(event.getX());
        }
    }

    virtual void handleDragEvent(int16_t /*oldX*/, int16_t /*oldY*/, int16_t newX, int16_t /*newY*/) override
    {
        updateValueFromX(newX);
    }

private:
    void updateDimensions()
    {
        uint16_t w = std::max<uint16_t>(backgroundX + backgroundBmp.getWidth(),
                                        indicatorMaxPosition + indicatorBmp.getWidth());
        uint16_t h = std::max<uint16_t>(backgroundY + backgroundBmp.getHeight(),
                                        indicatorBmp.getHeight());
        if (w > 0 && h > 0)
        {
            setWidthHeight(w, h);
        }
    }

    int16_t valueToPosition(int value) const
    {
        if (valueRangeMax <= valueRangeMin) return indicatorMinPosition;
        int32_t span = indicatorMaxPosition - indicatorMinPosition;
        return static_cast<int16_t>(indicatorMinPosition +
            ((value - valueRangeMin) * span) / (valueRangeMax - valueRangeMin));
    }

    void updateValueFromX(int16_t localX)
    {
        int16_t indRadius = indicatorBmp.getWidth() / 2;
        int32_t rel = localX - indRadius - indicatorMinPosition;
        int32_t span = indicatorMaxPosition - indicatorMinPosition;
        if (span <= 0) return;

        int32_t range = valueRangeMax - valueRangeMin;
        int newVal = valueRangeMin + static_cast<int>((rel * range + span / 2) / span);
        newVal = std::max(valueRangeMin, std::min(valueRangeMax, newVal));

        if (newVal != currentValue)
        {
            setValue(newVal);
            if (newValueCallback && newValueCallback->isValid())
            {
                newValueCallback->execute(*this, currentValue);
            }
        }
    }

    Bitmap backgroundBmp;
    Bitmap fillerBmp;
    Bitmap indicatorBmp;
    int currentValue;
    int valueRangeMin;
    int valueRangeMax;
    uint16_t backgroundX;
    uint16_t backgroundY;
    uint16_t fillerX;
    uint16_t fillerY;
    uint16_t indicatorMinPosition;
    uint16_t indicatorMaxPosition;

    GenericCallback<const Slider&, int>* startValueCallback;
    GenericCallback<const Slider&, int>* stopValueCallback;
    GenericCallback<const Slider&, int>* newValueCallback;
};

} // namespace touchgfx

#endif // TOUCHGFX_SLIDER_HPP
