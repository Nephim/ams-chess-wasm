#pragma once

#include "AbstractPiece.hpp"
#include <BitmapDatabase.hpp>

class Rook : public AbstractPiece {
public:
    Rook(PieceColor color, int position, Container* container)
        : AbstractPiece(color, PieceType::ROOK, Bitmap(color == PieceColor::WHITE ? BITMAP_WHITEROOK_ID : BITMAP_BLACKROOK_ID), container)
    {
        container->add(*_image);
        Move(position);
    }

    std::list<int> PotentialMoves(const std::shared_ptr<AbstractPiece> board[64], const int myPosition) const override {
        std::list<int> moves;
        int directions[4] = { -8, -1, 1, 8 }; // Rook can move vertically or horizontally

        for (int dir : directions) {
            int pos = myPosition;
            while (true) {
                pos += dir;
                // Check if the move is within bounds
                if (pos < 0 || pos >= 64 ||
                    (dir == -1 && pos % 8 == 7) || (dir == 1 && pos % 8 == 0)) {
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
