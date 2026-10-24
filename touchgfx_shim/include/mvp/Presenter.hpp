#ifndef TOUCHGFX_PRESENTER_HPP
#define TOUCHGFX_PRESENTER_HPP

namespace touchgfx
{

class Presenter
{
public:
    Presenter() {}
    virtual ~Presenter() {}
    virtual void activate() {}
    virtual void deactivate() {}
};

} // namespace touchgfx

#endif // TOUCHGFX_PRESENTER_HPP
