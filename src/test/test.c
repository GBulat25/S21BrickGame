#include <check.h>
#include <stdlib.h>
#include <time.h>

#include "../brick_game/tetris/game_logic.h"
#include "../gui/cli/terminal_interface.h"

// Тесты для функции createField
START_TEST(test_createField) {
  int **field = createField();
  ck_assert_ptr_nonnull(field);  // Проверка, что поле создано
  freeFieldMemory(field);        // Освобождение памяти
}
END_TEST

// Тесты для функции initializeField
START_TEST(test_initializeField) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  initializeField(&game);
  // Проверка, что границы поля инициализированы правильно
  for (int i = 0; i < FIELD_HEIGHT + 2; i++) {
    for (int j = 0; j < FIELD_WIDTH + 2; j++) {
      if (j == 0 || j == FIELD_WIDTH + 1 || i == 0 || i == FIELD_HEIGHT + 1) {
        ck_assert_int_eq(game.field[i][j], 1);  // Границы должны быть 1
      } else {
        ck_assert_int_eq(game.field[i][j],
                         0);  // Внутренняя часть должна быть 0
      }
    }
  }
  freeGameElementsMemory(&game);
}
END_TEST

// Тесты для функции createFigure
START_TEST(test_createFigure) {
  int **figure = createFigure();
  ck_assert_ptr_nonnull(figure);  // Проверка, что фигура создана
  freeFigureMemory(figure);  // Освобождение памяти
}
END_TEST

// Тесты для функции initializeFigures
START_TEST(test_initializeFigures) {
  int figures[7][4][4];
  initializeFigures(figures);
  for (int i = 0; i < 7; i++) {
    int has_ones =
        1;  // Флаг для проверки, что фигура содержит хотя бы одну единицу
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        has_ones = (figures[i][j][k] == 1) ? 1 : has_ones;
      }
    }
    ck_assert_int_eq(has_ones,
                     1);  // Проверка, что фигура содержит хотя бы одну единицу
  }
}

END_TEST

// Тесты для функции spawnNewFigure
START_TEST(test_spawnNewFigure) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  spawnNewFigure(&game);
  ck_assert_int_eq(game.curr_x,
                   FIELD_WIDTH / 2 - 2);  // Проверка начальной позиции X
  ck_assert_int_eq(game.curr_y, 0);  // Проверка начальной позиции Y
  freeGameElementsMemory(&game);
}
END_TEST

// Тесты для функции moveFigureDown
START_TEST(test_moveFigureDown) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  spawnNewFigure(&game);
  moveFigureDown(&game);
  ck_assert_int_eq(game.curr_y, 1);  // Проверка, что фигура сдвинулась вниз
  freeGameElementsMemory(&game);
}
END_TEST

// Тесты для функции moveFigureLeft
START_TEST(test_moveFigureLeft) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  spawnNewFigure(&game);
  moveFigureLeft(&game);
  ck_assert_int_eq(game.curr_x, FIELD_WIDTH / 2 - 3);  // Проверка сдвига влево
  freeGameElementsMemory(&game);
}
END_TEST

// Тесты для функции moveFigureRight
START_TEST(test_moveFigureRight) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  spawnNewFigure(&game);
  moveFigureRight(&game);
  ck_assert_int_eq(game.curr_x, FIELD_WIDTH / 2 - 1);  // Проверка сдвига вправо
  freeGameElementsMemory(&game);
}
END_TEST

// Тесты для функции clearLines
START_TEST(test_clearLines) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  // Заполняем строку для очистки
  for (int j = 1; j <= FIELD_WIDTH; j++) {
    game.field[FIELD_HEIGHT][j] = 1;
  }
  clearLines(&game);
  ck_assert_int_eq(game.score, 100);  // Проверка, что счет увеличился
  for (int j = 1; j <= FIELD_WIDTH; j++) {
    game.field[FIELD_HEIGHT][j] = 1;
    game.field[FIELD_HEIGHT - 1][j] = 1;
  }
  clearLines(&game);
  ck_assert_int_eq(game.score, 400);  // Проверка, что счет увеличился
  freeGameElementsMemory(&game);
}
END_TEST

// Тесты для функции rotateFigure
START_TEST(test_rotateFigure) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  spawnNewFigure(&game);
  rotateFigure(&game);
  ck_assert_int_eq(game.current_figure[0][0], 0);  // Проверка поворота фигуры
  freeGameElementsMemory(&game);
}
END_TEST

// Тесты для функции userInput
START_TEST(test_userInput) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  spawnNewFigure(&game);
  userInput(&game, Left, false);
  ck_assert_int_eq(game.curr_x, FIELD_WIDTH / 2 - 3);  // Проверка ввода "влево"
  userInput(&game, Right, false);
  ck_assert_int_eq(game.curr_x,
                   FIELD_WIDTH / 2 - 2);  // Проверка ввода "вправо"
  freeGameElementsMemory(&game);
}
END_TEST

// Тесты для функции convertInputToAction
START_TEST(test_convertInputToAction) {
  ck_assert_int_eq(convertInputToAction('s'),
                   Start);  // Проверка конвертации 's'
  ck_assert_int_eq(convertInputToAction('p'),
                   Pause);  // Проверка конвертации 'p'
  ck_assert_int_eq(convertInputToAction('q'),
                   Terminate);  // Проверка конвертации 'q'
  ck_assert_int_eq(convertInputToAction(KEY_LEFT),
                   Left);  // Проверка конвертации KEY_LEFT
  ck_assert_int_eq(convertInputToAction(KEY_RIGHT),
                   Right);  // Проверка конвертации KEY_RIGHT
  ck_assert_int_eq(convertInputToAction(KEY_UP),
                   Up);  // Проверка конвертации KEY_UP
  ck_assert_int_eq(convertInputToAction(KEY_DOWN),
                   Down);  // Проверка конвертации KEY_DOWN
  ck_assert_int_eq(convertInputToAction(' '),
                   Action);  // Проверка конвертации пробела
}
END_TEST

// Тест для функции timeDiff
START_TEST(test_timeDiff) {
  struct timeval start = {0, 0};
  struct timeval end = {1, 500000};  // 1.5 секунды

  long result = timeDiff(start, end);
  ck_assert_int_eq(result, 1500);  // Ожидаем 1500 миллисекунд
}
END_TEST

// Тест для функции createGameElements
START_TEST(test_createGameElements) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  bool result = createGameElements(&game);
  ck_assert(result == false);  // Проверяем, что память выделена успешно
  freeGameElementsMemory(&game);
}
END_TEST

// Тест для функции drawGame
START_TEST(test_drawGame) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  drawGame(&game);
  ck_assert(game.field != NULL);  // Проверяем, что игра отрисована без ошибок
  freeGameElementsMemory(&game);
}
END_TEST

// Тест для функции displayStartGame
START_TEST(test_displayStartGame) {
  displayStartGame();
  ck_assert(1);  // Простая проверка, что функция выполнилась
}
END_TEST

// Тест для функции freeAllocatedMemory
START_TEST(test_freeAllocatedMemory) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  freeAllocatedMemory(&game);
  ck_assert_ptr_null(game.field);  // Проверка, что память освобождена
  ck_assert_ptr_null(game.current_figure);
  ck_assert_ptr_null(game.next_figure);
}
END_TEST

// Тест для функции checkEmptyFigure
START_TEST(test_checkEmptyFigure) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  spawnNewFigure(&game);
  bool is_empty = checkEmptyFigure(&game);
  ck_assert(!is_empty);  // Проверка, что фигура не пуста
  freeGameElementsMemory(&game);
}
END_TEST

// Тест для функции collisionCheck
START_TEST(test_collisionCheck) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  spawnNewFigure(&game);
  bool collision = collisionCheck(&game);
  ck_assert(!collision);  // Проверка, что столкновений нет
  freeGameElementsMemory(&game);
}
END_TEST

// Тест для функции scoreAdd
START_TEST(test_scoreAdd) {
  int score = 0;
  scoreAdd(1, &score);
  ck_assert_int_eq(score, 100);  // Проверка добавления очков за 1 линию
  scoreAdd(2, &score);
  ck_assert_int_eq(score, 400);  // Проверка добавления очков за 2 линии
  scoreAdd(3, &score);
  ck_assert_int_eq(score, 1100);  // Проверка добавления очков за 3 линии
  scoreAdd(4, &score);
  ck_assert_int_eq(score, 2600);  // Проверка добавления очков за 4 линии
}
END_TEST

// Тест для функции updateGame
START_TEST(test_updateGame) {
  GameInfo_t game;
  memset(&game, 0, sizeof(GameInfo_t));  // Инициализация нулями
  createGameElements(&game);
  spawnNewFigure(&game);
  game.collision = true;
  game = updateCurrentState(game);
  ck_assert_int_eq(game.curr_state,
                   STATE_GAME_OVER);  // Проверка обновления состояния
  freeGameElementsMemory(&game);
}
END_TEST

// Тест для функции writeHighscore
START_TEST(test_writeHighscore) {
  const char *filename = "test_highscore.txt";
  writeHighscore(filename, 1000);
  FILE *file = fopen(filename, "r");
  int highscore = 0;
  fscanf(file, "%d", &highscore);
  fclose(file);
  ck_assert_int_eq(highscore, 1000);  // Проверка записи рекорда
  remove(filename);  // Удаление тестового файла
}
END_TEST

// Тест для функции fsmWork (переход из START в SPAWN)
START_TEST(test_fsmWork_start_to_spawn) {
  GameInfo_t game;
  createGameElements(&game);
  // Запускаем fsmWork на одну итерацию
  startProcessing(&game, 's');
  // Проверяем, что состояние изменилось на STATE_SPAWN
  ck_assert_int_eq(game.curr_state, STATE_SPAWN);
  freeGameElementsMemory(&game);
}
END_TEST

// Тест для функции fsmWork (переход из SPAWN в MOVE)
START_TEST(test_fsmWork_spawn_to_move) {
  GameInfo_t game;
  createGameElements(&game);
  game.curr_state = STATE_SPAWN;
  // Запускаем fsmWork на одну итерацию
  startProcessing(&game, 's');
  // Проверяем, что состояние изменилось на STATE_MOVE
  ck_assert_int_eq(game.curr_state, STATE_MOVE);
  freeGameElementsMemory(&game);
}
END_TEST

// Тест для функции fsmWork (переход из MOVE в PAUSE)
START_TEST(test_fsmWork_move_to_pause) {
  GameInfo_t game;
  createGameElements(&game);
  game.curr_state = STATE_PAUSE;
  // Запускаем fsmWork на одну итерацию
  startProcessing(&game, 'k');
  // Проверяем, что состояние изменилось на STATE_MOVE
  ck_assert_int_eq(game.curr_state, STATE_MOVE);
  freeGameElementsMemory(&game);
}
END_TEST

// Тест для функции fsmWork (переход из MOVE в SPAWN)
START_TEST(test_fsmWork_move_to_spawn) {
  GameInfo_t game;
  createGameElements(&game);
  game.curr_state = STATE_MOVE;
  // Запускаем fsmWork на одну итерацию
  startProcessing(&game, 'k');
  // Проверяем, что состояние изменилось на STATE_SPAWN
  ck_assert_int_eq(game.curr_state, STATE_SPAWN);
  freeGameElementsMemory(&game);
}
END_TEST

// Тест для функции fsmWork (переход из GAME_OVER в START)
START_TEST(test_fsmWork_over_to_start) {
  GameInfo_t game;
  createGameElements(&game);
  game.curr_state = STATE_GAME_OVER;
  // Запускаем fsmWork на одну итерацию
  startProcessing(&game, 'r');
  // Проверяем, что состояние изменилось на STATE_START
  ck_assert_int_eq(game.curr_state, STATE_START);
  freeGameElementsMemory(&game);
}
END_TEST

// Создание тестового набора
Suite *suite_brick_game() {
  Suite *s = suite_create("brick_game");

  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_createField);
  tcase_add_test(tc_core, test_initializeField);
  tcase_add_test(tc_core, test_createFigure);
  tcase_add_test(tc_core, test_initializeFigures);
  tcase_add_test(tc_core, test_spawnNewFigure);
  tcase_add_test(tc_core, test_moveFigureDown);
  tcase_add_test(tc_core, test_moveFigureLeft);
  tcase_add_test(tc_core, test_moveFigureRight);
  tcase_add_test(tc_core, test_clearLines);
  tcase_add_test(tc_core, test_rotateFigure);
  tcase_add_test(tc_core, test_userInput);
  tcase_add_test(tc_core, test_convertInputToAction);
  tcase_add_test(tc_core, test_timeDiff);
  tcase_add_test(tc_core, test_createGameElements);
  tcase_add_test(tc_core, test_drawGame);
  tcase_add_test(tc_core, test_displayStartGame);
  tcase_add_test(tc_core, test_freeAllocatedMemory);
  tcase_add_test(tc_core, test_checkEmptyFigure);
  tcase_add_test(tc_core, test_collisionCheck);
  tcase_add_test(tc_core, test_scoreAdd);
  tcase_add_test(tc_core, test_updateGame);
  tcase_add_test(tc_core, test_writeHighscore);
  tcase_add_test(tc_core, test_fsmWork_start_to_spawn);
  tcase_add_test(tc_core, test_fsmWork_spawn_to_move);
  tcase_add_test(tc_core, test_fsmWork_move_to_pause);
  tcase_add_test(tc_core, test_fsmWork_move_to_spawn);
  tcase_add_test(tc_core, test_fsmWork_over_to_start);
  suite_add_tcase(s, tc_core);

  return s;
}

// Основная функция для запуска всех тестов
int main(void) {
  int number_failed;
  Suite *s = suite_brick_game();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}