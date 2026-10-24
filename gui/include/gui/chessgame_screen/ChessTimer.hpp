#ifndef CHESSTIMER_HPP
#define CHESSTIMER_HPP

#include <gui/chessgame_screen/ChessEnums.hpp>
#include <touchgfx/Callback.hpp>
#include <cstdint>

class ChessTimer
{
public:
    ChessTimer(PieceColor currentPlayer);
    void setPlayer(PieceColor currentPlayer);
    void resume();
    void pause();
    void tick();
    void reset();

    void setWhiteTimeUpdateCallback(touchgfx::GenericCallback<uint8_t, uint8_t>* callback);
    void setBlackTimeUpdateCallback(touchgfx::GenericCallback<uint8_t, uint8_t>* callback);

    void handleTick(); // Called approximately 60 times per second

private:
    PieceColor _currentPlayer;
    uint32_t _whitePlayerTime; // Time in seconds
    uint32_t _blackPlayerTime; // Time in seconds
    uint32_t _tickCounter; // Counter to track ticks
    bool _isRunning;

    touchgfx::GenericCallback<uint8_t, uint8_t>* _whiteTimeUpdateCallback;
    touchgfx::GenericCallback<uint8_t, uint8_t>* _blackTimeUpdateCallback;
};

#endif /* CHESSTIMER_HPP */
