#ifndef CHESSAI_HPP
#define CHESSAI_HPP

#include <list>
#include <memory>
#include <gui/chessgame_screen/AbstractPiece.hpp>
#include <gui/chessgame_screen/ChessEnums.hpp>
#include <gui/chessgame_screen/BoardState.hpp>

class ChessAI {
public:
    ChessAI(PieceColor color, int depth = 3);

    std::pair<int, int> getBestMove(BoardState& boardState);

    void setDepth(int depth); // Function to set the depth

private:
    PieceColor aiColor;
    int searchDepth;

    int evaluateBoard(BoardState& boardState);
    int minimax(BoardState& boardState, int depth, int alpha, int beta, bool isMaximizingPlayer);

    std::list<std::pair<int, int>> getAllPossibleMoves(BoardState& boardState, PieceColor color);
};

#endif /* CHESSAI_HPP */
