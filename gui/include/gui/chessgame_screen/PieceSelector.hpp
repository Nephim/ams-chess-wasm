#pragma once

#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/containers/Container.hpp>
#include <list>
#include <vector>

using namespace touchgfx;

class PieceSelector : public Container {
public:
    PieceSelector();
    ~PieceSelector();
    void selectPiece(int position, const std::list<int>& possibleMoves, const std::list<int>& captureMoves);
    void deselectPiece();
    bool isVisible() const;
    bool isPossibleMove(int position) const; // Check if a position is a possible move

    // New methods to get possible and capture moves
    const std::list<int>& getPossibleMoves() const { return _possibleMoves; }
    const std::list<int>& getCaptureMoves() const { return _captureMoves; }

private:
    Box _pieceSelector;
    std::list<int> _possibleMoves; // Store possible moves
    std::list<int> _captureMoves; // Store capture moves
    std::vector<Box> _moveMarkers; // Markers for possible moves

    void clearMoveMarkers();
    void displayMoveMarkers(const std::list<int>& positions, const std::list<int>& captures);
};
