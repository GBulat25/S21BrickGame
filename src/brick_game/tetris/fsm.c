#include "../../gui/cli/terminal_interface.h"
#include "game_logic.h"
void fsmWork(GameInfo_t *game) {
  while (1) {
    int input = getch();  // Получение ввода пользователя
    startProcessing(game, input);
  }
  endwin();
}
long timeDiff(struct timeval start, struct timeval end) {
  // tv_sec-секунды, tv_usec-микросекунды
  return (end.tv_sec - start.tv_sec) * 1000 +
         (end.tv_usec - start.tv_usec) / 1000;
}
void startProcessing(GameInfo_t *game, int input) {
  // Логика для каждого состояния
  switch (game->curr_state) {
    case STATE_START:
      displayStartGame();
      if (input == 's') {
        game->curr_state = STATE_SPAWN;
      }
      break;
    case STATE_SPAWN:
      spawnNewFigure(game);
      game->curr_state = STATE_MOVE;
      break;
    case STATE_MOVE: {
      int interval = 1010 - game->level * 100;  // Интервал в миллисекундах
      struct timeval current_time;
      gettimeofday(&current_time, NULL);  // Получаем текущее время
      // Проверка, прошёл ли интервал с последнего вызова move_figure_down
      if (timeDiff(game->last_time, current_time) >= interval) {
        moveFigureDown(game);  // Движение фигуры вниз
        game->last_time = current_time;  // Обновление времени последнего вызова
      }

      UserAction_t action = convertInputToAction(input);
      userInput(game, action, false);
      *game = updateCurrentState(*game);
      drawGame(game);  // Отрисовка игры

      break;
    }
    case STATE_PAUSE:
      displayDirections();
      if (input == 'k') {
        game->curr_state = STATE_MOVE;  // Возврат в состояние MOVE
      }
      break;

    case STATE_GAME_OVER:
      displayGameOver();
      initializeField(game);  // Очистка поля
      game->score = 0;        // Сброс счета
      game->level = 1;
      if (input == 'r') {
        game->curr_state = STATE_START;  // Переход в состояние START
      }
      break;
      refresh();  // Обновление экрана
  }
}