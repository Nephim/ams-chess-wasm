#include <gui/chessgame_screen/ChessTimer.hpp>

ChessTimer::ChessTimer(PieceColor currentPlayer)
    : _currentPlayer(currentPlayer), _whitePlayerTime(0), _blackPlayerTime(0), _tickCounter(0), _isRunning(false),
    _whiteTimeUpdateCallback(nullptr), _blackTimeUpdateCallback(nullptr)
{}

void ChessTimer::setPlayer(PieceColor color)
{
    _currentPlayer = color;
}

void ChessTimer::resume()
{
    _isRunning = true;
    _tickCounter = 0;
}

void ChessTimer::pause()
{
    _isRunning = false;
}

void ChessTimer::tick()
{
    if (_isRunning)
    {
        _tickCounter++;
        if (_tickCounter >= 60) // Approximately 1 second has passed
        {
            _tickCounter = 0;
            if (_currentPlayer == PieceColor::WHITE)
            {
                _whitePlayerTime++;
                if (_whiteTimeUpdateCallback && _whiteTimeUpdateCallback->isValid())
                {
                    _whiteTimeUpdateCallback->execute(_whitePlayerTime / 60, _whitePlayerTime % 60);
                }
            }
            else
            {
                _blackPlayerTime++;
                if (_blackTimeUpdateCallback && _blackTimeUpdateCallback->isValid())
                {
                    _blackTimeUpdateCallback->execute(_blackPlayerTime / 60, _blackPlayerTime % 60);
                }
            }
        }
    }
}

void ChessTimer::reset()
{
    _whitePlayerTime = 0;
    _blackPlayerTime = 0;
    _currentPlayer = PieceColor::WHITE;
    _isRunning = false;
    _tickCounter = 0;

    if (_whiteTimeUpdateCallback && _whiteTimeUpdateCallback->isValid())
    {
        _whiteTimeUpdateCallback->execute(0, 0);
    }
    if (_blackTimeUpdateCallback && _blackTimeUpdateCallback->isValid())
    {
        _blackTimeUpdateCallback->execute(0, 0);
    }
}

void ChessTimer::setWhiteTimeUpdateCallback(touchgfx::GenericCallback<uint8_t, uint8_t>* callback)
{
    _whiteTimeUpdateCallback = callback;
}

void ChessTimer::setBlackTimeUpdateCallback(touchgfx::GenericCallback<uint8_t, uint8_t>* callback)
{
    _blackTimeUpdateCallback = callback;
}

void ChessTimer::handleTick()
{
    tick(); // Call the tick function every time handleTick is called
}
