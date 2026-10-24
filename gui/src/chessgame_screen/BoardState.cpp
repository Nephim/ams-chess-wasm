#include <gui/chessgame_screen/BoardState.hpp>
#include <algorithm>
#include <utility> // for std::move
#include <stdlib.h> // for abs

BoardState::BoardState()
    : _currentPlayer(PieceColor::WHITE),
    _lastMoveFrom(-1),
    _lastMoveTo(-1),
    _whiteKingPosition(-1),
    _blackKingPosition(-1)
{
    initialize();
}

void BoardState::initialize()
{
    for (auto& piece : _board) {
        piece.reset();
    }
}

void BoardState::reset()
{
    initialize();
    _currentPlayer = PieceColor::WHITE;
    _lastMoveFrom = -1;
    _lastMoveTo = -1;
    _whiteKingPosition = -1;
    _blackKingPosition = -1;
}

std::array<std::shared_ptr<AbstractPiece>, 64>& BoardState::getBoard()
{
    return _board;
}

void BoardState::setBoard(std::array<std::shared_ptr<AbstractPiece>, 64>&& board)
{
    _board = std::move(board);
    _lastMoveFrom = -1;
    _lastMoveTo = -1;
}

PieceColor BoardState::getCurrentPlayer() const
{
    return _currentPlayer;
}

void BoardState::setCurrentPlayer(PieceColor color)
{
    _currentPlayer = color;
}

int BoardState::getWhiteKingPosition() const
{
    return _whiteKingPosition;
}

void BoardState::setWhiteKingPosition(int position)
{
    _whiteKingPosition = position;
}

int BoardState::getBlackKingPosition() const
{
    return _blackKingPosition;
}

void BoardState::setBlackKingPosition(int position)
{
    _blackKingPosition = position;
}

int BoardState::getLastMoveFrom() const
{
    return _lastMoveFrom;
}

void BoardState::setLastMoveFrom(int position)
{
    _lastMoveFrom = position;
}

int BoardState::getLastMoveTo() const
{
    return _lastMoveTo;
}

void BoardState::setLastMoveTo(int position)
{
    _lastMoveTo = position;
}

int BoardState::isKingInCheck(PieceColor color)
{
    int kingPosition = (color == PieceColor::WHITE) ? _whiteKingPosition : _blackKingPosition;

    if (kingPosition == -1)
    {
        return -1; // King not found, should not happen
    }

    // Check if any opponent piece can move to the king's position
    for (int i = 0; i < 64; ++i)
    {
        auto& piece = _board[i];
        if (piece && piece->GetColor() != color)
        {
            std::list<int> possibleMoves = piece->PossibleMoves(_board.data(), i);
            if (std::find(possibleMoves.begin(), possibleMoves.end(), kingPosition) != possibleMoves.end())
            {
                return i; // Return the position of the checking piece
            }
        }
    }

    return -1; // No check
}

bool BoardState::wouldMoveCauseCheck(int from, int to)
{
    // Temporarily move the piece
    std::shared_ptr<AbstractPiece> tempPiece = std::move(_board[to]);
    _board[to] = std::move(_board[from]);
    _board[from] = nullptr;

    // Update king position if the king is moved
    if (_board[to] && _board[to]->GetType() == PieceType::KING)
    {
        if (_board[to]->GetColor() == PieceColor::WHITE)
        {
            _whiteKingPosition = to;
        }
        else
        {
            _blackKingPosition = to;
        }
    }

    bool isInCheck = isKingInCheck(_currentPlayer) != -1;

    // Undo the move
    _board[from] = std::move(_board[to]);
    _board[to] = std::move(tempPiece);

    // Restore king position if the king was moved
    if (_board[from] && _board[from]->GetType() == PieceType::KING)
    {
        if (_board[from]->GetColor() == PieceColor::WHITE)
        {
            _whiteKingPosition = from;
        }
        else
        {
            _blackKingPosition = from;
        }
    }

    return isInCheck;
}

bool BoardState::hasLegalMoves(PieceColor color)
{
    for (int i = 0; i < 64; ++i)
    {
        auto& piece = _board[i];
        if (piece && piece->GetColor() == color)
        {
            std::list<int> possibleMoves = piece->PossibleMoves(_board.data(), i);
            std::list<int> validMoves = filterValidMoves(possibleMoves, i);
            if (!validMoves.empty())
            {
                return true;
            }
        }
    }
    return false;
}

bool BoardState::hasCheckmate(PieceColor color)
{
    if (isKingInCheck(color) != -1 && !hasLegalMoves(color))
    {
        return true;
    }
    return false;
}

std::list<int> BoardState::filterValidMoves(const std::list<int>& possibleMoves, int from)
{
    std::list<int> validMoves;
    for (int to : possibleMoves)
    {
        if (!wouldMoveCauseCheck(from, to))
        {
            validMoves.push_back(to);
        }
    }
    // Check for castling specifically if the piece is a king
    if (_board[from] != nullptr && _board[from]->GetType() == PieceType::KING)
    {
        // Filter out invalid castling moves
        for (auto it = validMoves.begin(); it != validMoves.end(); )
        {
            int to = *it;
            if (abs(from - to) == 2) // Castling move detected
            {
                // Simulate moving the king to the intermediate square
                int intermediateSquare = (from + to) / 2;
                if (wouldMoveCauseCheck(from, intermediateSquare) || wouldMoveCauseCheck(from, to))
                {
                    it = validMoves.erase(it);
                    continue;
                }
            }
            ++it;
        }
    }
    return validMoves;
}
