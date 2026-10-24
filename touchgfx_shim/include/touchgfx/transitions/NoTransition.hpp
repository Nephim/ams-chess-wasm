#ifndef TOUCHGFX_TRANSITION_NOTRANSITION_HPP
#define TOUCHGFX_TRANSITION_NOTRANSITION_HPP

namespace touchgfx
{

class Transition
{
public:
    Transition() {}
    virtual ~Transition() {}
    virtual void init() {}
    virtual void tearDown() {}
    virtual bool isDone() const { return true; }
};

class NoTransition : public Transition
{
public:
    NoTransition() : Transition() {}
};

} // namespace touchgfx

#endif // TOUCHGFX_TRANSITION_NOTRANSITION_HPP
