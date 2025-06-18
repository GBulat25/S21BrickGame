#include "terminal_interface.h"
void initNcurses() {
  initscr(); // Инициализация ncurses
  cbreak();  // Отключение буферизации ввода
  noecho(); // Отключение отображения ввода пользователя
  keypad(stdscr,
         TRUE); // Включение поддержки функциональных клавиш (стрелки и т.д.)
  curs_set(0);  // Скрытие курсора
  nodelay(stdscr, TRUE); // Включаем неблокирующий ввод
  timeout(300);          // Устанавливаем таймаут
  start_color();
  initColorPairs();
}
void initColorPairs() {
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_YELLOW, COLOR_BLACK);
}

void drawGame(GameInfo_t *game) {
  timeout(300 - game->speed * 20); // Устанавливаем таймаут
  clear();                         // Очистка экрана
  // Отрисовка игрового поля
  drawField(game);
  // Отрисовка текущей фигуры
  drawCurrFigure(game);
  // отрисовка следующей фигуры
  drawNextFigure(game);
  // отрисовка счета
  mvprintw(2, 30, "Score: %d", game->score);
  mvprintw(3, 30, "Record: %d", game->high_score);
  mvprintw(4, 30, "Level: %d", game->level);
  refresh(); // Обновление экрана
}
void drawNextFigure(GameInfo_t *game) { // отрисовка следующей фигуры
  mvprintw(7, 30, "Next figure is:");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->next_figure[i][j]) {
        attron(COLOR_PAIR(game->next_figure_color)); // добавление цвета
        mvprintw(8 + i, 30 + j * 2, "[]"); // Отрисовка клетки фигуры
        attroff(COLOR_PAIR(game->next_figure_color)); // отключение цвета
      }
    }
  }
}
void drawCurrFigure(GameInfo_t *game) { // отрисовка текущей фигуры
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (game->current_figure[i][j]) {
        attron(COLOR_PAIR(game->curr_figure_color));
        int y = game->curr_y + i;
        int x = game->curr_x + j;
        mvprintw(y, x * 2, "[]"); // Отрисовка клетки фигуры
        attroff(COLOR_PAIR(game->curr_figure_color));
      }
    }
  }
}
void drawField(GameInfo_t *game) { // отрисовка игрового поля
  for (int i = 0; i <= FIELD_HEIGHT + 1; i++) {
    for (int j = 0; j <= FIELD_WIDTH + 1; j++) {
      if (game->field[i][j]) {
        mvprintw(i, j * 2, "[]"); // Отрисовка клетки (2 символа на клетку)
      } else {
        mvprintw(i, j * 2, "  "); // Пустая клетка
      }
    }
  }
}
void displayDirections() {
  clear();
  mvprintw(7, 20, "Game paused. Press 'k' to continue.");
  mvprintw(8, 20, "============Directions===========");
  mvprintw(9, 20, " Start new game ---> S      ");
  mvprintw(10, 20, " End the game -----> Q         ");
  mvprintw(11, 20, " Pause the game ---> P       ");
  mvprintw(12, 20, " Move left --------> Left arrow  ");
  mvprintw(13, 20, " Move right -------> Right arrow ");
  mvprintw(14, 20, " Accelerate -------> Down arrow  ");
  mvprintw(15, 20, " Rotate the piece -> SPACE           ");
}
void displayStartGame() {
  clear();
  mvprintw(7, 25, "               ");
  mvprintw(8, 25, "     press     ");
  mvprintw(9, 25, "      's'    ");
  mvprintw(10, 25, " to start game ");
  mvprintw(11, 25, "               ");
}
void displayGameOver() { mvprintw(22, 0, "Game over! Press 'r' to restart."); }