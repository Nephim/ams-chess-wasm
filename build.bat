@echo off
setlocal
cd /d "%~dp0"

if not exist dist mkdir dist

em++ -std=c++17 -O2 -Wno-inconsistent-missing-override ^
  -Itouchgfx_shim/include ^
  -Igui/include ^
  -Igenerated/gui_generated/include ^
  -Igenerated/images/include ^
  -Igenerated/texts/include ^
  -Igenerated/fonts/include ^
  -Irapidjson ^
  src/main_wasm.cpp ^
  touchgfx_shim/src/Unicode.cpp ^
  touchgfx_shim/src/TouchGFXCore.cpp ^
  touchgfx_shim/src/SDLBackend.cpp ^
  generated/gui_generated/src/chessgame_screen/ChessGameViewBase.cpp ^
  generated/gui_generated/src/common/FrontendApplicationBase.cpp ^
  generated/texts/src/LanguageGb.cpp ^
  generated/texts/src/Texts.cpp ^
  generated/texts/src/TypedTextDatabase.cpp ^
  gui/src/chessgame_screen/Board.cpp ^
  gui/src/chessgame_screen/BoardRenderer.cpp ^
  gui/src/chessgame_screen/BoardState.cpp ^
  gui/src/chessgame_screen/BoardStateModel.cpp ^
  gui/src/chessgame_screen/ChessAI.cpp ^
  gui/src/chessgame_screen/ChessGamePresenter.cpp ^
  gui/src/chessgame_screen/ChessGameView.cpp ^
  gui/src/chessgame_screen/ChessTimer.cpp ^
  gui/src/chessgame_screen/PieceSelector.cpp ^
  gui/src/chessgame_screen/SquareRenderer.cpp ^
  gui/src/common/FrontendApplication.cpp ^
  gui/src/common/GameStateSerializer.cpp ^
  gui/src/common/Snackbar.cpp ^
  gui/src/model/Model.cpp ^
  -sUSE_SDL=2 ^
  -sUSE_SDL_IMAGE=2 ^
  -sSDL2_IMAGE_FORMATS="[\"png\"]" ^
  -sUSE_SDL_TTF=2 ^
  -sALLOW_MEMORY_GROWTH=1 ^
  --preload-file assets@/assets ^
  --shell-file shell.html ^
  -o dist/index.html
