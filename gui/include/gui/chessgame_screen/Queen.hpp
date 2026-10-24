#pragma once

#include "AbstractPiece.hpp"
#include <BitmapDatabase.hpp>

class Queen : public AbstractPiece {
public:
    Queen(PieceColor color, int position, Container* container)
        : AbstractPiece(color, PieceType::QUEEN, Bitmap(color == PieceColor::WHITE ? BITMAP_WHITEQUEEN_ID : BITMAP_BLACKQUEEN_ID), container)
    {
        container->add(*_image);
        Move(position);
    }

    std::list<int> PotentialMoves(const std::shared_ptr<AbstractPiece> board[64], const int myPosition) const override {
        std::list<int> moves;
        int directions[8] = { -9, -8, -7, -1, 1, 7, 8, 9 }; // Queen can move in any direction

        for (int dir : directions) {
            int pos = myPosition;
            while (true) {
                pos += dir;
                // Check if the move is within bounds and on the correct diagonal
                if (pos < 0 || pos >= 64 ||
                    ((pos - dir) % 8 == 0 && (dir == -9 || dir == -1 || dir == 7)) ||
                    ((pos - dir) % 8 == 7 && (dir == -7 || dir == 1 || dir == 9))) {
                    break;
                }
                moves.push_back(pos);
                // Stop if there is a piece in the way
                if (board[pos] != nullptr) {
                    break;
                }
            }
        }

        return moves;
    }

    std::list<int> PossibleMoves(const std::shared_ptr<AbstractPiece> board[64], const int myPosition) const override {
        std::list<int> potentialMoves = PotentialMoves(board, myPosition);
        std::list<int> possibleMoves;

        for (int move : potentialMoves) {
            // Add logic to check for valid moves considering other pieces
            if (board[move] == nullptr || board[move]->GetColor() != this->color) {
                possibleMoves.push_back(move);
            }
        }

        return possibleMoves;
    }
};
