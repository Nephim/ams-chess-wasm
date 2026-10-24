#ifndef GAMESTATESERIALIZER_HPP
#define GAMESTATESERIALIZER_HPP

#include <string>
#include <array>
#include <memory>
#include <gui/chessgame_screen/AbstractPiece.hpp>
#include <gui/chessgame_screen/ChessEnums.hpp>
#include <gui/chessgame_screen/BoardStateModel.hpp>
#include <gui/chessgame_screen/BoardRenderer.hpp>
#include <document.h>

class GameStateSerializer {
public:
    GameStateSerializer();

    std::string SerializeGameState(BoardStateModel& boardStateModel);
    BoardStateModel DeserializeGameState(const std::string& serializedGameState, BoardRenderer& boardRenderer);

private:
    PieceColor DeserializeCurrentPlayer(const rapidjson::Document& document);
    std::array<std::shared_ptr<AbstractPiece>, 64> DeserializeBoardState(const rapidjson::Document& document, BoardRenderer& boardRenderer);
};

#endif /* GAMESTATESERIALIZER_HPP */
