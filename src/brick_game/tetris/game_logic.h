#ifndef GAME_LOGIC_H  // Если BRICK_GAME_H не определен
#define GAME_LOGIC_H  // Определяем BRICK_GAME_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>        // Для работы с временем
#define FIELD_WIDTH 10   // ширина поля
#define FIELD_HEIGHT 20  // высота поля
typedef struct {
  int **field;  // игровое поле
  int all_figures[7][4][4];  // массив для хранения всех типов фигур
  int **current_figure;   // текущая фигура
  int **next_figure;      // следующая фигура
  int curr_x, curr_y;     // текущие координаты фигуры
  int score;              // счет
  int curr_state;         // текущее состояние игры
  int curr_figure_color;  // цвет текущей фигуры
  int next_figure_color;  // цвет следующей фигуры
  struct timeval last_time;  // Время последнего вызова move_figure_down
  int high_score;  // рекорд
  int level;       // уровень
  bool collision;  // информация было ли столкновение фигуры с чем либо
  int speed;       // скорость игры
  int pause;       // информация о паузе
} GameInfo_t;  // структура для хранения информации игры
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;  // действия пользователя
typedef enum {
  STATE_START,        // Начальное состояние
  STATE_SPAWN,        // Создание новой фигуры
  STATE_MOVE,         // Фигура движется
  STATE_PAUSE,        // Пауза
  STATE_GAME_OVER     // Игра завершена
} game_state;         // состояния игры
int **createField();  // выделение памяти для  поля
void freeFieldMemory(int **field);  // освобождения памяти, выделенной для поля
void initializeField(GameInfo_t *game);  // инициализация игрового поля и границ
void initializeFigures(int figures[7][4][4]);  // создание всех типов фигур
int **createFigure();  // выделение памяти для фигуры
void freeFigureMemory(
    int **figure);  // освобождение памяти, выделенной для фигуры
bool createGameElements(
    GameInfo_t *game);  // инициализация элементов игры для старта
void freeAllocatedMemory(GameInfo_t *game);  // освобождение памяти в случае
                                             // проблем с выделением памяти
void freeGameElementsMemory(GameInfo_t *game);  // освобождение памяти
                                                // выделенной для элементов игры
void chooseFigure(GameInfo_t *game);  // выбор рандомной фигуры
bool checkEmptyFigure(
    GameInfo_t *game);  // проверка, инициализировалась ли фигура
void copyNextToCurFigure(
    GameInfo_t *game);  // копируем показанную фигуру в текущую
void spawnNewFigure(GameInfo_t *game);  // создание новой фигуры
void moveFigureDown(GameInfo_t *game);   // движение вниз
void moveFigureLeft(GameInfo_t *game);   // движение влево
void moveFigureRight(GameInfo_t *game);  // движение вправо
bool collisionCheck(
    GameInfo_t *game);  // проверка, было ли столкновение фигуры с чем-либо
void clearLines(GameInfo_t *game);  // уничтожение заполненных линий
void scoreAdd(int score, int *score_to_add);  // увеличение счета
void rotateFigure(GameInfo_t *game);          // вращение фигуры
void fsmWork(GameInfo_t *game);  // обработка действий в различных состояниях
long timeDiff(struct timeval start,
              struct timeval end);  // проверка, прошел ли указанный интервал
int readHighscore(const char *filename);  // получение рекорда
void writeHighscore(const char *filename,
                    int highscore);  // установка рекорда
void userInput(GameInfo_t *game, UserAction_t action,
               bool hold);  // обработка действий пользователя
GameInfo_t updateCurrentState(
    GameInfo_t game);  // получение текущего состояния поля
UserAction_t convertInputToAction(
    int input);  // конвертация ввода пользователя для дальнейшей обработки
void startProcessing(GameInfo_t *game, int input);
void shiftLines(int *i, GameInfo_t *game,
                int *score);  // сдвиг всех линий выше
void collisionTrue(GameInfo_t *game, int i, int j,
                   bool *res);  // обнаружение столкновения
void updateOnCollision(GameInfo_t *game, int *i,
                       int *j);  // обновление поля при столкновении
#endif
