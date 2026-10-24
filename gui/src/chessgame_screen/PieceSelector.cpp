#include <gui/chessgame_screen/PieceSelector.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/containers/Container.hpp>
#include <algorithm>

#define MARKER_SIZE 8
#define MARKER_OFFSET (34 - MARKER_SIZE) / 2

PieceSelector::PieceSelector()
    : _pieceSelector(34, 34, touchgfx::Color::getColorFromRGB(174, 198, 207), 200),
    _moveMarkers(64) // Initialize vector to hold 64 Box markers
{
    setWidth(272);
    setHeight(272);

    _pieceSelector.setVisible(false);
    add(_pieceSelector); // Add the piece selector to the container

    // Initialize and position the move markers
    for (int i = 0; i < 64; i++) {
        _moveMarkers[i] = Box(MARKER_SIZE, MARKER_SIZE, touchgfx::Color::getColorFromRGB(0, 0, 0), 100);
        _moveMarkers[i].setVisible(false);
        _moveMarkers[i].moveTo((i % 8) * 34 + MARKER_OFFSET, (i / 8) * 34 + MARKER_OFFSET);
        add(_moveMarkers[i]);
    }
}

void PieceSelector::selectPiece(int position, const std::list<int>& possibleMoves, const std::list<int>& captureMoves) {
    _pieceSelector.moveTo((position % 8) * 34, (position / 8) * 34);
    _pieceSelector.setVisible(true);
    _pieceSelector.invalidate();

    clearMoveMarkers();
    displayMoveMarkers(possibleMoves, captureMoves);
}

void PieceSelector::deselectPiece() {
    _pieceSelector.setVisible(false);
    _pieceSelector.invalidate();
    clearMoveMarkers();
}

bool PieceSelector::isVisible() const {
    return _pieceSelector.isVisible();
}

void PieceSelector::clearMoveMarkers() {
    for (auto& marker : _moveMarkers) {
        marker.setVisible(false);
    }
    _possibleMoves.clear();
    _captureMoves.clear();
}

void PieceSelector::displayMoveMarkers(const std::list<int>& positions, const std::list<int>& captures) {
    for (int pos : positions) {
        if (pos < 0 || pos >= 64) {
            continue; // Safety check to ensure valid positions
        }
        _moveMarkers[pos].setVisible(true);
        _moveMarkers[pos].invalidate();
        _possibleMoves.push_back(pos);
    }
    for (int pos : captures) {
        if (pos < 0 || pos >= 64) {
            continue; // Safety check to ensure valid positions
        }
        _captureMoves.push_back(pos);
    }
}

bool PieceSelector::isPossibleMove(int position) const {
    return std::find(_possibleMoves.begin(), _possibleMoves.end(), position) != _possibleMoves.end() ||
        std::find(_captureMoves.begin(), _captureMoves.end(), position) != _captureMoves.end();
}

PieceSelector::~PieceSelector() {
    clearMoveMarkers();
}
