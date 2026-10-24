#pragma once

#include "AbstractPiece.hpp"
#include <BitmapDatabase.hpp>

class Knight : public AbstractPiece {
public:
    Knight(PieceColor color, int position, Container* container)
        : AbstractPiece(color, PieceType::KNIGHT, Bitmap(color == PieceColor::WHITE ? BITMAP_WHITEHORSE_ID : BITMAP_BLACKHORSE_ID), container)
    {
        container->add(*_image);
        Move(position);
    }

    std::list<int> PotentialMoves(const std::shared_ptr<AbstractPiece> board[64], const int myPosition) const override {
        // Implementation for potential moves of a KNIGHT
        std::list<int> moves;
        int directions[8] = { -17, -15, -10, -6, 6, 10, 15, 17 }; // L-shaped moves

        for (int dir : directions) {
            int pos = myPosition + dir;
            // Check if the move is within bounds
            if (pos >= 0 && pos < 64 &&
                !((myPosition % 8 == 0 && (dir == -17 || dir == -10 || dir == 6 || dir == 15)) ||
                    (myPosition % 8 == 1 && (dir == -10 || dir == 6)) ||
                    (myPosition % 8 == 6 && (dir == -6 || dir == 10)) ||
                    (myPosition % 8 == 7 && (dir == -15 || dir == -6 || dir == 10 || dir == 17)))) {
                moves.push_back(pos);
            }
        }

        return moves;
    }

    std::list<int> PossibleMoves(const std::shared_ptr<AbstractPiece> board[64], const int myPosition) const override {
        // Implementation for possible moves of a KNIGHT
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
