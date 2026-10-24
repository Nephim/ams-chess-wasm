#pragma once

#include <gui/chessgame_screen/ChessEnums.hpp>
#include <list>
#include <memory>

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>
#include <touchgfx/containers/Container.hpp>

using namespace touchgfx;

class AbstractPiece {
protected:
    PieceColor color; // Indicates whether the piece is white or black
    PieceType type; // Symbol representing the piece on the board
    std::shared_ptr<ScalableImage> _image; // Image of the piece
    Container* _container;
    bool hasMoved = false;

public:
    AbstractPiece(PieceColor c, PieceType t, const Bitmap& bmp, Container* container)
        : color(c), type(t), _image(std::make_unique<ScalableImage>(bmp)), _container(container)
    {}

    virtual ~AbstractPiece() {
        _container->remove(*_image);
    };

    // All moves inside the board if no other piece is blocking the way
    virtual std::list<int> PotentialMoves(const std::shared_ptr<AbstractPiece> board[64], const int myPosition) const = 0;

    // Moves that are possible to make, considering the board and other pieces
    virtual std::list<int> PossibleMoves(const std::shared_ptr<AbstractPiece> board[64], const int myPosition) const = 0;

    virtual PieceColor GetColor() const {
        return color;
    }

    virtual PieceType GetType() const {
        return type;
    }

    virtual ScalableImage* GetImage() const {
        return _image.get();
    }

    void Move(int position) const {
        int x = (position % 8) * 34;
        int y = (position / 8) * 34;

        _image->setPosition(x, y, 33, 33);
        _image->invalidate();
    }

	void SetMoved() {
		hasMoved = true;
	}

	bool HasMoved() const {
		return hasMoved;
	}
};
