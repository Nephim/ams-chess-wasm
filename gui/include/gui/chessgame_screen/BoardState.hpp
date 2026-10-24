#ifndef BOARDSTATE_HPP
#define BOARDSTATE_HPP

#include <array>
#include <memory>
#include <list>
#include <gui/chessgame_screen/AbstractPiece.hpp>
#include <gui/chessgame_screen/ChessEnums.hpp>
#include <gui/chessgame_screen/BoardStateModel.hpp>

class BoardState {
public:
    BoardState();

    void initialize();
    void reset();

    std::array<std::shared_ptr<AbstractPiece>, 64>& getBoard();
    void setBoard(std::array<std::shared_ptr<AbstractPiece>, 64>&& board);

    PieceColor getCurrentPlayer() const;
    void setCurrentPlayer(PieceColor color);

    int getWhiteKingPosition() const;
    void setWhiteKingPosition(int position);

    int getBlackKingPosition() const;
    void setBlackKingPosition(int position);

    int getLastMoveFrom() const;
    void setLastMoveFrom(int position);

    int getLastMoveTo() const;
    void setLastMoveTo(int position);

    bool hasLegalMoves(PieceColor color);
    bool hasCheckmate(PieceColor color);
    bool wouldMoveCauseCheck(int from, int to);
    int isKingInCheck(PieceColor color);

    std::list<int> filterValidMoves(const std::list<int>& possibleMoves, int from);

private:
    std::array<std::shared_ptr<AbstractPiece>, 64> _board;
    PieceColor _currentPlayer;
    int _lastMoveFrom;
    int _lastMoveTo;
    int _whiteKingPosition;
    int _blackKingPosition;
};

#endif /* BOARDSTATE_HPP */
