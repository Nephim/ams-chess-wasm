#include "gui/common/GameStateSerializer.hpp"

#include <rapidjson.h>
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include <string>
#include "filereadstream.h"
#include "error/en.h"
#include <cstdio>
#include <gui/chessgame_screen/Pawn.hpp>
#include <gui/chessgame_screen/Rook.hpp>
#include <gui/chessgame_screen/Knight.hpp>
#include <gui/chessgame_screen/Bishop.hpp>
#include <gui/chessgame_screen/Queen.hpp>
#include <gui/chessgame_screen/King.hpp>
#ifndef SIMULATOR
#include "fatfs.h"
#endif


GameStateSerializer::GameStateSerializer() {}

std::string GameStateSerializer::SerializeGameState(BoardStateModel& boardStateModel) {
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    // Serialize the board
    rapidjson::Value boardArray(rapidjson::kArrayType);
    for (const auto& piece : boardStateModel.getBoard()) {
        if (piece) {
            rapidjson::Value pieceObject(rapidjson::kObjectType);
            pieceObject.AddMember("type", static_cast<int>(piece->GetType()), allocator);
            pieceObject.AddMember("color", static_cast<int>(piece->GetColor()), allocator);
            boardArray.PushBack(pieceObject, allocator);
        }
        else {
            boardArray.PushBack(rapidjson::Value(rapidjson::kNullType), allocator);
        }
    }
    document.AddMember("board", boardArray, allocator);
    document.AddMember("currentPlayer", static_cast<int>(boardStateModel.getCurrentPlayer()), allocator);

    // Convert JSON document to string
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    std::string gameStateString = buffer.GetString();

#ifndef SIMULATOR
    WriteToSDCARD(gameStateString.c_str());
#endif


    return gameStateString;

}

BoardStateModel GameStateSerializer::DeserializeGameState(const std::string& serializedGameState, BoardRenderer& boardRenderer) {
    
	std::string _serializedGameState = serializedGameState;

#ifndef SIMULATOR
    _serializedGameState = ReadFromSDCARD();
	if (_serializedGameState.empty()) {
        _serializedGameState = serializedGameState;
	}
#endif


    rapidjson::Document document;
    if (document.Parse(_serializedGameState.c_str()).HasParseError()) {
        printf("Error(offset %u): %s\n",
            (unsigned)document.GetErrorOffset(),
            rapidjson::GetParseError_En(document.GetParseError()));
        return BoardStateModel();
    }

    BoardStateModel boardStateModel;
    boardStateModel.setBoard(DeserializeBoardState(document, boardRenderer));
    boardStateModel.setCurrentPlayer(DeserializeCurrentPlayer(document));

    return boardStateModel;
}

PieceColor GameStateSerializer::DeserializeCurrentPlayer(const rapidjson::Document& document) {
    if (document.HasMember("currentPlayer") && document["currentPlayer"].IsInt()) {
        return static_cast<PieceColor>(document["currentPlayer"].GetInt());
    }
    return PieceColor::WHITE; // Default to white if not found
}

std::array<std::shared_ptr<AbstractPiece>, 64> GameStateSerializer::DeserializeBoardState(const rapidjson::Document& document, BoardRenderer& boardRenderer) {
    std::array<std::shared_ptr<AbstractPiece>, 64> tempboard;

    if (document.HasMember("board") && document["board"].IsArray()) {
        const rapidjson::Value& boardArray = document["board"];
        for (rapidjson::SizeType i = 0; i < boardArray.Size(); ++i) {
            const rapidjson::Value& piece = boardArray[i];
            if (!piece.IsNull() && piece.IsObject()) {
                if (piece.HasMember("type") && piece["type"].IsInt() &&
                    piece.HasMember("color") && piece["color"].IsInt()) {
                    PieceType type = static_cast<PieceType>(piece["type"].GetInt());
                    PieceColor color = static_cast<PieceColor>(piece["color"].GetInt());

                    switch (type) {
                    case PieceType::PAWN:
                        tempboard[i] = std::make_unique<Pawn>(color, i, &boardRenderer);
                        break;
                    case PieceType::KING:
                        tempboard[i] = std::make_unique<King>(color, i, &boardRenderer);
                        break;
                    case PieceType::QUEEN:
                        tempboard[i] = std::make_unique<Queen>(color, i, &boardRenderer);
                        break;
                    case PieceType::ROOK:
                        tempboard[i] = std::make_unique<Rook>(color, i, &boardRenderer);
                        break;
                    case PieceType::BISHOP:
                        tempboard[i] = std::make_unique<Bishop>(color, i, &boardRenderer);
                        break;
                    case PieceType::KNIGHT:
                        tempboard[i] = std::make_unique<Knight>(color, i, &boardRenderer);
                        break;
                    default:
                        break;
                    }
                }
            }
            else {
                tempboard[i] = nullptr;
            }
        }
    }

    return tempboard;
}
