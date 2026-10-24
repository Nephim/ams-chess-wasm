#ifndef TOUCHGFX_DIGITALCLOCK_HPP
#define TOUCHGFX_DIGITALCLOCK_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/SDLBackend.hpp>
#include <cstdio>

namespace touchgfx
{

class DigitalClock : public Container
{
public:
    enum DisplayMode
    {
        DISPLAY_12_HOUR_NO_SECONDS,
        DISPLAY_24_HOUR_NO_SECONDS,
        DISPLAY_12_HOUR,
        DISPLAY_24_HOUR
    };

    DigitalClock()
        : Container(),
          typedText(TYPED_TEXT_INVALID),
          color(0),
          displayMode(DISPLAY_24_HOUR_NO_SECONDS),
          useLeadingZeroForHourIndicator(true),
          currentHour(0),
          currentMinute(0),
          currentSecond(0),
          alpha(255)
    {
        buffer[0] = 0;
        updateClockText();
    }

    virtual ~DigitalClock() {}

    void setTypedText(TypedText t)
    {
        typedText = t;
        updateClockText();
    }

    void setColor(colortype c)
    {
        color = c;
        invalidate();
    }

    void setDisplayMode(DisplayMode dm)
    {
        displayMode = dm;
        updateClockText();
    }

    void displayLeadingZeroForHourIndicator(bool displayLeadingZero)
    {
        useLeadingZeroForHourIndicator = displayLeadingZero;
        updateClockText();
    }

    void setTime24Hour(uint8_t hour, uint8_t minute, uint8_t second)
    {
        currentHour = hour;
        currentMinute = minute;
        currentSecond = second;
        updateClockText();
        invalidate();
    }

    void setAlpha(uint8_t a) { alpha = a; }
    uint8_t getAlpha() const { return alpha; }

    virtual void draw(int16_t parentAbsX, int16_t parentAbsY, uint8_t parentAlpha = 255) const override
    {
        if (!visible || alpha == 0 || parentAlpha == 0 || !typedText.hasValidId()) return;
        uint8_t effAlpha = static_cast<uint8_t>((static_cast<uint16_t>(alpha) * parentAlpha) / 255);
        SDLBackend::drawText(typedText, buffer, nullptr,
                             parentAbsX + rect.x, parentAbsY + rect.y,
                             rect.width, rect.height, color, effAlpha, false);
    }

protected:
    void updateClockText()
    {
        char tmp[16];
        if (displayMode == DISPLAY_24_HOUR_NO_SECONDS || displayMode == DISPLAY_12_HOUR_NO_SECONDS)
        {
            if (useLeadingZeroForHourIndicator)
                std::snprintf(tmp, sizeof(tmp), "%02u:%02u", static_cast<unsigned>(currentHour), static_cast<unsigned>(currentMinute));
            else
                std::snprintf(tmp, sizeof(tmp), "%u:%02u", static_cast<unsigned>(currentHour), static_cast<unsigned>(currentMinute));
        }
        else
        {
            if (useLeadingZeroForHourIndicator)
                std::snprintf(tmp, sizeof(tmp), "%02u:%02u:%02u", static_cast<unsigned>(currentHour), static_cast<unsigned>(currentMinute), static_cast<unsigned>(currentSecond));
            else
                std::snprintf(tmp, sizeof(tmp), "%u:%02u:%02u", static_cast<unsigned>(currentHour), static_cast<unsigned>(currentMinute), static_cast<unsigned>(currentSecond));
        }
        Unicode::strncpy(buffer, tmp, BUFFER_SIZE);
        buffer[BUFFER_SIZE - 1] = 0;
    }

    static const uint16_t BUFFER_SIZE = 16;
    Unicode::UnicodeChar buffer[BUFFER_SIZE];
    TypedText typedText;
    colortype color;
    DisplayMode displayMode;
    bool useLeadingZeroForHourIndicator;
    uint8_t currentHour;
    uint8_t currentMinute;
    uint8_t currentSecond;
    uint8_t alpha;
};

} // namespace touchgfx

#endif // TOUCHGFX_DIGITALCLOCK_HPP
