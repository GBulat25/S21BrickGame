#include "../../brick_game/tetris/game_logic.h"
#include "terminal_interface.h"

int main() {
  GameInfo_t game;
  // Проверяем, возможно ли выделить память для игровых элементов
  if (!createGameElements(&game)) {
    srand(time(NULL));  // Инициализация генератора случайных чисел
    gettimeofday(&game.last_time, NULL);  // Устанавливаем текущее время
    initNcurses();  // инициализируем интерфейс
    fsmWork(&game);
    freeGameElementsMemory(&game);  // Освобождаем память
  } else {
    perror("Memory fail");  // Вывод ошибки, если память не выделена
  }

  return 0;
}