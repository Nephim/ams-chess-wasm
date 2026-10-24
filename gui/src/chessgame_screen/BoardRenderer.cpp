#include <gui/chessgame_screen/BoardRenderer.hpp>
#include <gui/chessgame_screen/Pawn.hpp>
#include <gui/chessgame_screen/Rook.hpp>
#include <gui/chessgame_screen/Knight.hpp>
#include <gui/chessgame_screen/Bishop.hpp>
#include <gui/chessgame_screen/Queen.hpp>
#include <gui/chessgame_screen/King.hpp>

BoardRenderer::BoardRenderer() {
    setWidth(272);
    setHeight(272);
}

void BoardRenderer::setupBoard(std::array<std::shared_ptr<AbstractPiece>, 64>& board) {
    // Initialize BLACK pawns
    for (int j = 0; j < 8; ++j) {
        int position = j + 8;
        addPiece(std::make_unique<Pawn>(PieceColor::BLACK, position, this), position, board);
    }

    // Initialize WHITE pawns
    for (int j = 0; j < 8; ++j) {
        int position = 6 * 8 + j;
        addPiece(std::make_unique<Pawn>(PieceColor::WHITE, position, this), position, board);
    }

    // Initialize other pieces
    // WHITE Bishops
    int bishopPositions[] = { 2, 5 };
    for (int j = 0; j < 2; ++j) {
        int position = 7 * 8 + bishopPositions[j];
        addPiece(std::make_unique<Bishop>(PieceColor::WHITE, position, this), position, board);
    }

    // BLACK Bishops
    for (int j = 0; j < 2; ++j) {
        int position = bishopPositions[j];
        addPiece(std::make_unique<Bishop>(PieceColor::BLACK, position, this), position, board);
    }

    // WHITE Knights
    int knightPositions[] = { 1, 6 };
    for (int j = 0; j < 2; ++j) {
        int position = 7 * 8 + knightPositions[j];
        addPiece(std::make_unique<Knight>(PieceColor::WHITE, position, this), position, board);
    }

    // BLACK Knights
    for (int j = 0; j < 2; ++j) {
        int position = knightPositions[j];
        addPiece(std::make_unique<Knight>(PieceColor::BLACK, position, this), position, board);
    }

    // WHITE Queen
    int position = 7 * 8 + 3;
    addPiece(std::make_unique<Queen>(PieceColor::WHITE, position, this), position, board);

    // BLACK Queen
    position = 3;
    addPiece(std::make_unique<Queen>(PieceColor::BLACK, position, this), position, board);

    // WHITE King
    position = 7 * 8 + 4;
    addPiece(std::make_unique<King>(PieceColor::WHITE, position, this), position, board);

    // BLACK King
    position = 4;
    addPiece(std::make_unique<King>(PieceColor::BLACK, position, this), position, board);

    // WHITE Rooks
    int rookPositions[] = { 0, 7 };
    for (int j = 0; j < 2; ++j) {
        position = 7 * 8 + rookPositions[j];
        addPiece(std::make_unique<Rook>(PieceColor::WHITE, position, this), position, board);
    }

    // BLACK Rooks
    for (int j = 0; j < 2; ++j) {
        position = rookPositions[j];
        addPiece(std::make_unique<Rook>(PieceColor::BLACK, position, this), position, board);
    }
}

void BoardRenderer::setupSavedBoard(std::array<std::shared_ptr<AbstractPiece>, 64>& board, std::array<std::shared_ptr<AbstractPiece>, 64>& tmpboard)
{
    for (auto& piece : board) {
        piece.reset();  // This sets the shared_ptr to nullptr
    }
    for (int i = 0; i < 64; ++i) {
        if (tmpboard[i]) {
            addPiece(std::move(tmpboard[i]), i, board);
        }
    }
}

void BoardRenderer::promotePawn(std::array<std::shared_ptr<AbstractPiece>, 64>& board, int position)
{
	PieceColor color = board[position]->GetColor();
	board[position].reset();
	addPiece(std::make_unique<Queen>(color, position, this), position, board);

}

void BoardRenderer::addPiece(std::shared_ptr<AbstractPiece> piece, int position, std::array<std::shared_ptr<AbstractPiece>, 64>& board) {
    board[position] = std::move(piece);
}
