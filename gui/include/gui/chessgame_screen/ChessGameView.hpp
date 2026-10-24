#ifndef CHESSGAMEVIEW_HPP
#define CHESSGAMEVIEW_HPP

#include <gui_generated/chessgame_screen/ChessGameViewBase.hpp>
#include <gui/chessgame_screen/ChessGamePresenter.hpp>
#include <gui/chessgame_screen/Board.hpp>
#include <gui/chessgame_screen/ChessTimer.hpp>
#include <touchgfx/Callback.hpp>

enum GameState {
    MENU,
    GAME
};

class ChessGameView : public ChessGameViewBase
{
public:
    ChessGameView();
    virtual ~ChessGameView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();

    // Override the difficulty slider value changed function
    virtual void DifficultyValueChanged(int value) override;
    virtual void WinScreenButton() override;

    // Declaring callback handler for Box
    void screenClickedHandler(const Image& i, const ClickEvent& e);
    void buttonCallbackHandler(const ButtonWithIcon& src, const ClickEvent& e);
    void gameModeButtonCallbackHandler(const TextButtonStyle<IconButtonStyle<BoxWithBorderButtonStyle<ClickButtonTrigger>>>& src, const ClickEvent& e);

    // Timer set UI
    void setWhiteTimer(uint8_t minutes, uint8_t seconds);
    void setBlackTimer(uint8_t minutes, uint8_t seconds);

    // Game state management
    void switchToMenu();
    void switchToGame(bool aiMode = false, uint8_t aiDifficulty = 3);
    void handleGameOver();

protected:
    // Declaring callback type of ScalableImage and clickEvent
    Callback<ChessGameView, const Image&, const ClickEvent&> screenClickedCallback;
    Callback<ChessGameView, const ButtonWithIcon&, const ClickEvent&> buttonClickedCallback;
    Callback<ChessGameView, const TextButtonStyle<IconButtonStyle<BoxWithBorderButtonStyle<ClickButtonTrigger>>>&, const ClickEvent&> gameModeButtonCallback;

    ScalableImage _testpawn;
    // Add Board to view
    Board _chessboard;
    ChessTimer _chessTimer;  // ChessTimer instance

    // TouchGFX callbacks for timer updates and player turn
    Callback<ChessGameView, uint8_t, uint8_t> whiteTimeUpdateCallback;
    Callback<ChessGameView, uint8_t, uint8_t> blackTimeUpdateCallback;
    Callback<ChessGameView, PieceColor> playerTurnCallback;
    Callback<ChessGameView, PieceColor> winnerCallback;

    void whiteTimerUpdated(uint8_t minutes, uint8_t seconds);
    void blackTimerUpdated(uint8_t minutes, uint8_t seconds);
    void updatePlayerTurn(PieceColor color);
    void updateWinner(PieceColor color);

    GameState currentState;
    int aiDepth; // Variable to store the AI depth
};

#endif // CHESSGAMEVIEW_HPP
