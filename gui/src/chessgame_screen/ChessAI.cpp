#include <gui/chessgame_screen/ChessAI.hpp>
#include <algorithm> // For std::max and std::min
#include <limits>

ChessAI::ChessAI(PieceColor color, int depth)
    : aiColor(color), searchDepth(depth) {}

std::pair<int, int> ChessAI::getBestMove(BoardState& boardState) {
    int bestValue = std::numeric_limits<int>::min();
    std::pair<int, int> bestMove = { -1, -1 };

    std::list<std::pair<int, int>> possibleMoves = getAllPossibleMoves(boardState, aiColor);
    for (const auto& move : possibleMoves) {
        BoardState newState = boardState;
        newState.getBoard()[move.second] = std::move(newState.getBoard()[move.first]);
        newState.getBoard()[move.first] = nullptr;

        // Ensure king position updates if a king is moved
        if (newState.getBoard()[move.second] && newState.getBoard()[move.second]->GetType() == PieceType::KING) {
            if (newState.getBoard()[move.second]->GetColor() == PieceColor::WHITE) {
                newState.setWhiteKingPosition(move.second);
            }
            else {
                newState.setBlackKingPosition(move.second);
            }
        }

        int boardValue = minimax(newState, searchDepth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
        if (boardValue > bestValue) {
            bestValue = boardValue;
            bestMove = move;
        }
    }

    return bestMove;
}

void ChessAI::setDepth(int depth) {
    searchDepth = depth;
}

int ChessAI::evaluateBoard(BoardState& boardState) {
    // A simple evaluation function: sum of piece values
    int pieceValues[6] = { 100, 300, 300, 500, 900, 10000 };
    int evaluation = 0;

    for (const auto& piece : boardState.getBoard()) {
        if (piece) {
            int value = pieceValues[static_cast<int>(piece->GetType())];
            evaluation += (piece->GetColor() == aiColor) ? value : -value;
        }
    }

    return evaluation;
}

int ChessAI::minimax(BoardState& boardState, int depth, int alpha, int beta, bool isMaximizingPlayer) {
    if (depth == 0) {
        return evaluateBoard(boardState);
    }

    PieceColor currentPlayer = isMaximizingPlayer ? aiColor : (aiColor == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE);
    std::list<std::pair<int, int>> possibleMoves = getAllPossibleMoves(boardState, currentPlayer);

    if (possibleMoves.empty()) {
        return isMaximizingPlayer ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    }

    if (isMaximizingPlayer) {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& move : possibleMoves) {
            BoardState newState = boardState;
            newState.getBoard()[move.second] = std::move(newState.getBoard()[move.first]);
            newState.getBoard()[move.first] = nullptr;

            // Ensure king position updates if a king is moved
            if (newState.getBoard()[move.second] && newState.getBoard()[move.second]->GetType() == PieceType::KING) {
                if (newState.getBoard()[move.second]->GetColor() == PieceColor::WHITE) {
                    newState.setWhiteKingPosition(move.second);
                }
                else {
                    newState.setBlackKingPosition(move.second);
                }
            }

            int eval = minimax(newState, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    }
    else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& move : possibleMoves) {
            BoardState newState = boardState;
            newState.getBoard()[move.second] = std::move(newState.getBoard()[move.first]);
            newState.getBoard()[move.first] = nullptr;

            // Ensure king position updates if a king is moved
            if (newState.getBoard()[move.second] && newState.getBoard()[move.second]->GetType() == PieceType::KING) {
                if (newState.getBoard()[move.second]->GetColor() == PieceColor::WHITE) {
                    newState.setWhiteKingPosition(move.second);
                }
                else {
                    newState.setBlackKingPosition(move.second);
                }
            }

            int eval = minimax(newState, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

std::list<std::pair<int, int>> ChessAI::getAllPossibleMoves(BoardState& boardState, PieceColor color) {
    std::list<std::pair<int, int>> moves;

    for (int i = 0; i < 64; ++i) {
        auto& piece = boardState.getBoard()[i];
        if (piece && piece->GetColor() == color) {
            std::list<int> possibleMoves = piece->PossibleMoves(boardState.getBoard().data(), i);
            for (int to : possibleMoves) {
                moves.emplace_back(i, to);
            }
        }
    }

    return moves;
}
