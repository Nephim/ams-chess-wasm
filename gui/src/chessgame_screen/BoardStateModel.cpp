#include <gui/chessgame_screen/BoardStateModel.hpp>

BoardStateModel::BoardStateModel()
    : _currentPlayer(PieceColor::WHITE) {
    for (auto& piece : _board) {
        piece.reset();
    }
}

std::array<std::shared_ptr<AbstractPiece>, 64>& BoardStateModel::getBoard() {
    return _board;
}

void BoardStateModel::setBoard(const std::array<std::shared_ptr<AbstractPiece>, 64>& board) {
    _board = board;
}

PieceColor BoardStateModel::getCurrentPlayer() const {
    return _currentPlayer;
}

void BoardStateModel::setCurrentPlayer(PieceColor color) {
    _currentPlayer = color;
}

void BoardStateModel::resetBoard() {
    for (auto& piece : _board) {
        piece = nullptr;
    }
}