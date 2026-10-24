#include <gui/chessgame_screen/SquareRenderer.hpp>

const colortype SquareRenderer::LIGHT_SQUARE_COLOR = Color::getColorFromRGB(240, 217, 181);
const colortype SquareRenderer::DARK_SQUARE_COLOR = Color::getColorFromRGB(181, 136, 99);

SquareRenderer::SquareRenderer() {
    setWidth(272);
    setHeight(272);
    initializeSquares();
}

void SquareRenderer::initializeSquares() {
    for (int i = 0; i < 64; ++i) {
        _squares[i].setPosition((i % 8) * 34, (i / 8) * 34, 34, 34);
        colortype color = ((i / 8) % 2 == (i % 8) % 2) ? LIGHT_SQUARE_COLOR : DARK_SQUARE_COLOR;
        _squares[i].setColor(color);
        add(_squares[i]);
    }
}

void SquareRenderer::updateSquareColor(int index, colortype color) {
    if (index >= 0 && index < 64) {
        _squares[index].setColor(color);
        _squares[index].invalidate();
    }
}

void SquareRenderer::resetSquareColors() {
    for (int i = 0; i < 64; ++i) {
        colortype color = ((i / 8) % 2 == (i % 8) % 2) ? LIGHT_SQUARE_COLOR : DARK_SQUARE_COLOR;
        _squares[i].setColor(color);
    }
    invalidate();
}
