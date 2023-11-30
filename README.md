# chess
A chess program using c++ and SFML/TGUI libraries.
### figures
`Figure` class have functions of checking if a move is correct and getting all correct moves.
\
`King` and `Rook` have additional functions for castle.
### game_components
Game components contain classes for chess clock, players and history in algebraic notation.
### game_state
Game field uses a matrix 8x8 of ptrs to the `Figure`. It has functions for saving/loading a game by json and making a move.
### game_graphics
Has a `render()` function for drawing the game window.
### tests
Contains tests for `GameState` class, which check correctness of various game moves. It uses Catch2.
