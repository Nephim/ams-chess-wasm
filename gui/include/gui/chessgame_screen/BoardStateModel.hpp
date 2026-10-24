#ifndef BOARDSTATEMODEL_HPP
#define BOARDSTATEMODEL_HPP

#include <array>
#include <memory>
#include <utility>
#include <gui/chessgame_screen/AbstractPiece.hpp>
#include <gui/chessgame_screen/ChessEnums.hpp>

class BoardStateModel {
public:
    BoardStateModel();

    std::array<std::shared_ptr<AbstractPiece>, 64>& getBoard();
    void setBoard(const std::array<std::shared_ptr<AbstractPiece>, 64>& board);

    PieceColor getCurrentPlayer() const;
    void setCurrentPlayer(PieceColor color);

    void resetBoard();

private:
    std::array<std::shared_ptr<AbstractPiece>, 64> _board;
    PieceColor _currentPlayer;
};

#endif /* BOARDSTATEMODEL_HPP */
