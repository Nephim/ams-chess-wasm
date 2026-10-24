#ifndef CHESSGAMEPRESENTER_HPP
#define CHESSGAMEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ChessGameView;

class ChessGamePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ChessGamePresenter(ChessGameView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ChessGamePresenter() {}

private:
    ChessGamePresenter();

    ChessGameView& view;
};

#endif // CHESSGAMEPRESENTER_HPP
