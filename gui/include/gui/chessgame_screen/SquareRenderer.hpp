#ifndef SQUARERENDERER_HPP
#define SQUARERENDERER_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/containers/Container.hpp>
#include <array>

using namespace touchgfx;

class SquareRenderer : public Container
{
public:
    SquareRenderer();
    void initializeSquares();
    void updateSquareColor(int index, colortype color);
    void resetSquareColors();
    static const colortype LIGHT_SQUARE_COLOR;
    static const colortype DARK_SQUARE_COLOR;

private:
    std::array<Box, 64> _squares; // Array to represent the board squares
};

#endif /* SQUARERENDERER_HPP */
