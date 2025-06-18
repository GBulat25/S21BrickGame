#include "game_logic.h"

#include "../../gui/cli/terminal_interface.h"

//  Создание игровых элементов (поле, фигуры)
bool createGameElements(GameInfo_t *game) {
  game->score = 0;
  game->level = 0;
  game->speed = 0;
  game->curr_state = STATE_START;
  game->high_score =
      readHighscore("db/high_score_record.txt");  // Чтение рекорда
  int error_flag = false;
  // Выделение памяти для игрового поля и фигур
  game->field = createField();  // Инициализация игрового поля
  game->current_figure = createFigure();  // Создание текущей фигуры
  game->next_figure = createFigure();  // Создание следующей фигуры
  // Проверка, удалось ли выделить память
  if (game->field == NULL || game->current_figure == NULL ||
      game->next_figure == NULL) {
    // Освобождение памяти, если выделение не удалось
    error_flag = true;
    freeAllocatedMemory(game);
  } else {
    initializeField(game);  // Инициализация игрового поля
    initializeFigures(game->all_figures);  // Инициализация всех фигур
  }
  return error_flag;
}
// Освобождение памяти в случае ошибки
void freeAllocatedMemory(GameInfo_t *game) {
  if (game->field) {
    freeFieldMemory(game->field);
    game->field = NULL;
  }
  if (game->current_figure) {
    freeFigureMemory(game->current_figure);
    game->current_figure = NULL;
  }
  if (game->next_figure) {
    freeFigureMemory(game->next_figure);
    game->next_figure = NULL;
  }
}
// Освобождение памяти, выделенной для игровых элементов
void freeGameElementsMemory(GameInfo_t *game) {
  freeFieldMemory(game->field);  // Освобождение памяти для поля
  freeFigureMemory(
      game->current_figure);  // Освобождение памяти для текущей фигуры
  freeFigureMemory(
      game->next_figure);  // Освобождение памяти для следующей фигуры
}
// Создание игрового поля
int **createField() {
  // размер полей увеличен на 2, так как добавляются границы
  bool error_flag = false;
  int **field = (int **)calloc(FIELD_HEIGHT + 2, sizeof(int *));
  if (field == NULL) {
    error_flag = true;
  }
  for (int i = 0; i < FIELD_HEIGHT + 2; i++) {
    field[i] = (int *)calloc(FIELD_WIDTH + 2, sizeof(int));
    if (field[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(field[j]);
      }
      free(field);
      error_flag = true;
    }
  }
  if (error_flag) {
    field = NULL;
  }
  return field;
}
// Освобождение памяти для игрового поля
void freeFieldMemory(int **field) {
  if (field == NULL) return;
  for (int i = 0; i < FIELD_HEIGHT + 2; i++) {
    free(field[i]);
  }
  free(field);
}
// Инициализация игрового поля нулями, границ единицами
void initializeField(GameInfo_t *game) {
  for (int i = 0; i < FIELD_HEIGHT + 2; i++) {
    for (int j = 0; j < FIELD_WIDTH + 2; j++) {
      if (j == 0 || j == FIELD_WIDTH + 1 || i == 0 || i == FIELD_HEIGHT + 1) {
        game->field[i][j] = 1;
      } else {
        game->field[i][j] = 0;
      }
    }
  }
}
// Создание фигуры
int **createFigure() {
  bool error_flag = false;
  int **figure = (int **)calloc(4, sizeof(int *));
  if (figure == NULL) {
    error_flag = true;
  }
  for (int i = 0; i < 4; i++) {
    figure[i] = (int *)calloc(4, sizeof(int));
    if (figure[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(figure[j]);
      }
      free(figure);
      error_flag = true;
    }
  }
  if (error_flag) {
    figure = NULL;
  }
  return figure;
}
// Освобождение памяти для фигуры
void freeFigureMemory(int **figure) {
  if (figure == NULL) return;
  for (int i = 0; i < 4; i++) {
    free(figure[i]);
  }
  free(figure);
}
// Инициализация всех возможных фигур (I, O, T, S, Z, J, L)
void initializeFigures(int figures[7][4][4]) {
  int I_figure[4][4] = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  memcpy(figures[0], I_figure, sizeof(I_figure));  // Копирование фигуры I

  int O_figure[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  memcpy(figures[1], O_figure, sizeof(O_figure));  // Копирование фигуры O

  int T_figure[4][4] = {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};
  memcpy(figures[2], T_figure, sizeof(T_figure));  // Копирование фигуры T

  int S_figure[4][4] = {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}};
  memcpy(figures[3], S_figure, sizeof(S_figure));  // Копирование фигуры S

  int Z_figure[4][4] = {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}};
  memcpy(figures[4], Z_figure, sizeof(Z_figure));  // Копирование фигуры Z

  int J_figure[4][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};
  memcpy(figures[5], J_figure, sizeof(J_figure));  // Копирование фигуры J

  int L_figure[4][4] = {{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}};
  memcpy(figures[6], L_figure, sizeof(L_figure));  // Копирование фигуры L
}
// Генерация фигуры при ошибке
void chooseFigure(GameInfo_t *game) {
  int new_figure_number = rand() % 7;  // Случайный выбор фигуры
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->next_figure[i][j] = game->all_figures[new_figure_number][i][j];
    }
  }
  int new_color_number = 1 + rand() % 6;
  game->next_figure_color = new_color_number;
}
// Проверка, инициациализировалась ли фигура
bool checkEmptyFigure(GameInfo_t *game) {
  bool empty_flag = true;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->current_figure[i][j] == 1) {
        empty_flag = false;
        i = 4;
        j = 4;
      }
    }
  }
  return empty_flag;
}
// Копируем показанную фигуру в текущую
void copyNextToCurFigure(GameInfo_t *game) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->current_figure[i][j] = game->next_figure[i][j];
    }
  }
  game->curr_figure_color = game->next_figure_color;
}
// Генерация новой фигуры на поле
void spawnNewFigure(GameInfo_t *game) {
  game->curr_x = FIELD_WIDTH / 2 - 2;  // Начальная позиция по X (центр)
  game->curr_y = 0;  // Начальная позиция по Y (верх)
  copyNextToCurFigure(game);
  if (checkEmptyFigure(game)) {
    chooseFigure(game);
    copyNextToCurFigure(game);
  }
  chooseFigure(game);
  game->collision = false;
}

// Перемещение фигуры вниз
void moveFigureDown(GameInfo_t *game) {
  game->curr_y++;
  if (collisionCheck(game)) {  // Проверка столкновения
    game->curr_y -= 1;  // Отмена сдвига, если есть столкновение
    game->collision = true;
  } else {
    game->curr_y++;
    if (collisionCheck(game)) {  // Проверка столкновения
      game->collision = true;
    }
    game->curr_y -= 1;  // Отмена сдвига, если есть столкновение
  }
}

// Перемещение фигуры влево
void moveFigureLeft(GameInfo_t *game) {
  game->curr_x--;  // Сдвиг фигуры влево
  if (collisionCheck(game) || game->collision) {  // Проверка столкновения
    game->curr_x++;  // Отмена сдвига, если есть столкновение
  }
}

// Перемещение фигуры вправо
void moveFigureRight(GameInfo_t *game) {
  game->curr_x++;  // Сдвиг фигуры вправо
  if (collisionCheck(game) || game->collision) {  // Проверка столкновения
    game->curr_x--;  // Отмена сдвига, если есть столкновение
  }
}

// Проверка столкновений фигуры с границами поля или другими фигурами
bool collisionCheck(GameInfo_t *game) {
  bool res = false;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->current_figure[i][j]) {
        collisionTrue(game, i, j, &res);
      }
    }
  }
  return res;
}
void collisionTrue(GameInfo_t *game, int i, int j, bool *res) {
  int x = game->curr_x + j;  // Текущая позиция X
  int y = game->curr_y + i;  // Текущая позиция Y
  if (x <= 0 || x >= FIELD_WIDTH + 1 || y >= FIELD_HEIGHT + 1 ||
      game->field[y][x]) {
    *res = true;
  }
}
// Удаление заполненных линий и обновление счета
void clearLines(GameInfo_t *game) {
  int score = 0;  // Счетчик удаленных линий
  for (int i = FIELD_HEIGHT; i > 0; i--) {
    bool line_full = true;
    for (int j = 1; j <= FIELD_WIDTH; j++) {
      if (game->field[i][j] == 0) {
        line_full = false;  // Линия не заполнена
      }
    }
    if (line_full) {
      // Сдвиг всех линий выше вниз
      shiftLines(&i, game, &score);
    }
  }
  scoreAdd(score, &(game->score));  // Обновление счета
  // увеличение скорости и уровня
  if (game->score >= 600 * (game->level)) {
    game->level++;
    game->speed++;
  }
  // увеличиваем рекорд
  if (game->score > game->high_score) {
    writeHighscore("db/high_score_record.txt", game->score);
    game->high_score = game->score;
  }
}
// Сдвиг всех линий
void shiftLines(int *i, GameInfo_t *game, int *score) {
  for (int k = *i; k > 1; k--) {
    for (int j = 1; j <= FIELD_WIDTH; j++) {
      game->field[k][j] = game->field[k - 1][j];
    }
  }
  *i += 1;
  *score += 1;  // Увеличение счетчика удаленных линий
}
// Добавление очков в зависимости от количества удаленных линий
void scoreAdd(int score, int *score_to_add) {
  switch (score) {
    case 1:
      *score_to_add += 100;
      break;
    case 2:
      *score_to_add += 300;
      break;
    case 3:
      *score_to_add += 700;
      break;
    case 4:
      *score_to_add += 1500;
      break;
    default:
      break;
  }
}
// Поворот фигуры
void rotateFigure(GameInfo_t *game) {
  bool rotate_fail = false;
  int temp[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp[j][3 - i] = game->current_figure[i][j];
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int x = game->curr_x + j;  // Текущая позиция X
      int y = game->curr_y + i;  // Текущая позиция Y
      if (temp[i][j] && (x <= 0 || x >= FIELD_WIDTH + 1 ||
                         y >= FIELD_HEIGHT + 1 || game->field[y][x])) {
        rotate_fail = true;  // Поворот невозможен
      }
    }
  }
  // Применение поворота, если он возможен
  if (!rotate_fail) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        game->current_figure[i][j] = temp[i][j];  // Обновление фигуры
      }
    }
  }
}
int readHighscore(const char *filename) {
  FILE *file = fopen(filename, "r");
  int highscore = 0;
  fscanf(file, "%d", &highscore);
  fclose(file);
  return highscore;
}
void writeHighscore(const char *filename, int highscore) {
  FILE *file = fopen(filename, "w");
  if (!file) {
    perror("Failed to open highscore file");
    return;
  }
  fprintf(file, "%d", highscore);
  fclose(file);
}
void userInput(GameInfo_t *game, UserAction_t action, bool hold) {
  switch (action) {
    case Left:
      moveFigureLeft(game);
      break;
    case Right:
      moveFigureRight(game);
      break;
    case Down:
      if (hold) {
        while (!game->collision) {
          moveFigureDown(game);
          drawGame(game);
          napms(50);
        }
      } else {
        moveFigureDown(game);
      }
      break;
    case Action:
      rotateFigure(game);
      break;
    case Terminate:
      game->curr_state = STATE_GAME_OVER;
      break;
    case Pause:
      game->curr_state = STATE_PAUSE;
      break;
    case Start:
      game->curr_state = STATE_START;
      break;
    case Up:
      break;
  }
}
GameInfo_t updateCurrentState(GameInfo_t game) {
  if (game.collision) {  // Проверка столкновений
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        updateOnCollision(&game, &i, &j);
      }
    }
    if (game.curr_state != STATE_GAME_OVER) {
      clearLines(&game);  // Удаление заполненных линий
      game.curr_state = STATE_SPAWN;
    }
  }
  return game;
}
void updateOnCollision(GameInfo_t *game, int *i, int *j) {
  if (game->current_figure[*i][*j]) {
    int x = game->curr_x + *j;  // Текущая позиция X
    int y = game->curr_y + *i;  // Текущая позиция Y
    if (y <= 1) {
      game->curr_state = STATE_GAME_OVER;
      *i = 4;
      *j = 4;
    } else {
      game->field[y][x] = 1;  // Закрепление фигуры на поле
    }
  }
}
UserAction_t convertInputToAction(int input) {
  UserAction_t keycap = -1;
  switch (input) {
    case 's':  // Клавиша 's' для старта
      keycap = Start;
      break;
    case 'p':  // Клавиша 'p' для паузы
      keycap = Pause;
      break;
    case 'q':  // Клавиша 'q' для завершения
      keycap = Terminate;
      break;
    case KEY_LEFT:  // Стрелка влево
      keycap = Left;
      break;
    case KEY_RIGHT:  // Стрелка вправо
      keycap = Right;
      break;
    case KEY_UP:  // Стрелка вверх
      keycap = Up;
      break;
    case KEY_DOWN:  // Стрелка вниз
      keycap = Down;
      break;
    case ' ':  // Пробел для действия
      keycap = Action;
      break;
  }
  return keycap;
}
