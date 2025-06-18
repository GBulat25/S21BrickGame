# Настройки компилятора
CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra
LDFLAGS = -lncurses

# Определение ОС
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    LDFLAGS += -lcheck -lm --coverage
else
    LDFLAGS += -lcheck -lsubunit -lm --coverage
endif

# Директории
SRC_TETRIS = brick_game/tetris
SRC_CLI = gui/cli
SRC_TEST = test
OBJ_DIR = obj
BIN_DIR = bin

# Исходные файлы
TETRIS_SRCS = $(SRC_TETRIS)/game_logic.c $(SRC_TETRIS)/fsm.c
TETRIS_OBJS = $(patsubst $(SRC_TETRIS)/%.c, $(OBJ_DIR)/%.o, $(TETRIS_SRCS))

CLI_SRCS = $(SRC_CLI)/terminal_interface.c
CLI_OBJS = $(patsubst $(SRC_CLI)/%.c, $(OBJ_DIR)/%.o, $(CLI_SRCS))

TEST_SRCS = $(SRC_TEST)/test.c
TEST_OBJS = $(patsubst $(SRC_TEST)/%.c, $(OBJ_DIR)/%.o, $(TEST_SRCS))

# Имя исполняемого файла
EXECUTABLE = $(BIN_DIR)/tetris

# Установочный каталог (рабочий стол)
INSTALL_DIR ?= $(HOME)/Desktop

# Цели по умолчанию
all: $(EXECUTABLE)

# Сборка библиотеки tetris
libtetris.a: $(TETRIS_OBJS)
	ar rcs $@ $^
	ranlib $@

# Сборка терминального интерфейса
$(EXECUTABLE): libtetris.a $(CLI_OBJS) $(OBJ_DIR)/main.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CLI_OBJS) $(OBJ_DIR)/main.o -L. -ltetris $(LDFLAGS) -o $@

# Компиляция .c файлов в .o
$(OBJ_DIR)/%.o: $(SRC_TETRIS)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) --coverage -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_CLI)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) --coverage -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_TEST)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) --coverage -c $< -o $@

# Создание директорий для объектных файлов и бинарников
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Установка программы
install: $(EXECUTABLE)
	mkdir -p $(INSTALL_DIR)
	cp $(EXECUTABLE) $(INSTALL_DIR)

# Удаление программы
uninstall:
	rm -f $(INSTALL_DIR)/tetris

# Очистка проекта
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) *.a *.gcno *.gcda *.gcov gcov_report coverage.info

# Запуск программы
run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Тестирование
test: $(TEST_OBJS) $(TETRIS_OBJS) $(CLI_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(TETRIS_OBJS) $(CLI_OBJS) $(LDFLAGS) -o $(BIN_DIR)/test
	./$(BIN_DIR)/test

# Генерация отчета покрытия
gcov_report: test
	mkdir -p gcov_report
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '*/tests/*' '*/usr/*' --output-file coverage.info --ignore-errors unused
	genhtml coverage.info --output-directory gcov_report
	@if [ "$(UNAME)" = "Darwin" ]; then \
		open gcov_report/index.html; \
	elif [ "$(UNAME)" = "Linux" ]; then \
		if grep -iq microsoft /proc/version; then \
			echo "You are using a terminal in Windows OS (WSL)!"; \
			echo "You can open html report using cmd command line enter command 'start gcov_report/index.html' ..."; \
		else \
			xdg-open gcov_report/index.html; \
		fi \
	else \
		echo "Unsupported OS"; \
	fi

# Проверка стиля кода
style:
	clang-format -n $(SRC_TETRIS)/*.h $(SRC_TETRIS)/*.c $(SRC_CLI)/*.c $(SRC_CLI)/*.h $(SRC_TEST)/*.h $(SRC_TEST)/*.c

# Форматирование кода
format:
	clang-format -i $(SRC_TETRIS)/*.h $(SRC_TETRIS)/*.c $(SRC_CLI)/*.c $(SRC_CLI)/*.h $(SRC_TEST)/*.h $(SRC_TEST)/*.c

# Генерация документации
dvi:
	mkdir -p docs
	pandoc info.md -o docs/info.tex

# Создание дистрибутива
dist:
	mkdir -p dist/tetris
	cp -r $(SRC_TETRIS) $(SRC_CLI) $(SRC_TEST) Makefile  dist/tetris
	tar -czvf tetris.tar.gz -C dist .
	rm -rf dist

.PHONY: all clean install uninstall run test gcov_report style format dvi dist