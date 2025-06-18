#ifndef INTERFACE
#define INTERFACE
#include <ncurses.h>

#include "../../brick_game/tetris/game_logic.h"
void initNcurses();  // инициализация ncurses
void drawGame(GameInfo_t *game);  // отрисовка текущего состояния игры
void initColorPairs();  // инициализация цветов
void drawNextFigure(GameInfo_t *game);  // отрисовка следующей фигуры
void drawCurrFigure(GameInfo_t *game);  // отрисовка текущей фигуры
void drawField(GameInfo_t *game);  // отрисовка поля
void displayDirections();  // вывод инструкции при паузе
void displayStartGame();   // вывод на экран при старте
void displayGameOver();  // вывод на экран при окончании игры
#endif