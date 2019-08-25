# Backgammon

This is an implementation of the famous board game, using C++ and SFML on win32.

## How to build?

The project only supports [cmake](https://cmake.org/) currently.

Please select either one of (**BKG_USE_SFML**, **BKG_USE_GLFW**) to choose the implementation of the GUI. 
Currently the selectable pieces are only available using the SFML implementation.

## How to play?

Run **Backgammon.exe**

Assuming you have knowledge of Backgammon:
  * The console window displays useful information such as whose turn it is and dice-roll results.
  * **[Space]** to roll the dice, the board will be highlighted in **Pink**.
  * **[Left-Click]** to pick up a piece, the board will be highlighted in **Turquoise**.
  * Available moves are highlighted, **[Left-Click]** to place the piece.
  * **[Right-Click]** to return a piece if it's picked up, and another **[Right-Click]** to skip the turn (*in case there are no available moves*).
  * If all your pieces are in your Home Board, you've begun "**Bearing Off**".
  * If a piece can be taken off the board, the board will be highlighted in **Red**, **[Left-Click]** to take it off.
  
## Licence
[GNU GPLv3](https://choosealicense.com/licenses/gpl-3.0/)
