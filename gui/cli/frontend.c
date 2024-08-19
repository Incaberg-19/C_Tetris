/// \file
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "../../brick_game/tetris/brick.h"

/**
 * @brief print object
 * @param[in] objects list of objects
 * @param[in] x
 * @param[in] y
 * @param[in] numberObject
 * @details
 * print object by coordinates, clear the next figure zone
 */
void printObject(int (*objects)[5][5], int x, int y, int numberObject) {
  if (x > COLUMS) {
    attrset(A_NORMAL);
    for (int i = y; i < y + 3; i++) {
      for (int j = x; j < x + 5; j++) {
        mvprintw(i, j * 2, "  ");
      }
    }
  }

  for (int i = y, k = 0; i < y + 5; i++, k++) {
    for (int j = x, c = 0; j < x + 5; j++, c++) {
      if (objects[numberObject][k][c] != SPACE) {
        attron(COLOR_PAIR(objects[numberObject][k][c]));
        if (objects[numberObject][k][c] == 9 &&
            x > COLUMS) {  // условие для правильного отображения
          mvprintw(i - 1, (j - 1) * 2, "%d%d", objects[numberObject][k][c],
                   objects[numberObject][k][c]);  // next синего объекта
        } else {
          mvprintw(i, j * 2, "%d%d", objects[numberObject][k][c],
                   objects[numberObject][k][c]);
        }
      }
    }
  }
  attrset(A_NORMAL);
}

void printGameKeys() {
  mvprintw(2, 52, "ARROWS MOVEMENT");
  mvprintw(4, 52, "SHIFT + ARDOWN - BOOST");
  mvprintw(6, 52, "SPACEBAR - PAUSE");
  mvprintw(8, 52, "R - ROTATE");
  mvprintw(10, 52, "Q - QUIT");
}

void printFrame() {
  attron(COLOR_PAIR(WALL));
  mvvline(1, 28, '7', ROWS);  // делаем границы правой формы
  mvvline(1, 48, '7', ROWS);
  mvhline(ROWS, 28, '7', 20);
  mvhline(6, 28, '7', 20);
  mvhline(1, 29, '7', 20);
  mvhline(12, 29, '7', 20);
  attroff(COLOR_PAIR(WALL));
  attron(A_BOLD);
  mvprintw(7, 34, "INCOMING!");
  mvprintw(2, 29, "SCORE:");
  mvprintw(4, 29, "HIGHSCORE:");
  mvprintw(14, 36, "LEVEL");
  attroff(A_BOLD);
}

/**
 * @brief print field
 * @param[in] game game current status
 */
void printField(GameInfo_t *game) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMS; j++) {
      attron(COLOR_PAIR(game->field[i][j]));
      mvprintw(i + 1, (j + 1) * 2, "%d%d", game->field[i][j],
               game->field[i][j]);
    }
  }
  attrset(A_NORMAL);
}

void initCollors() {
  start_color();

  init_color(COLOR_DARKBLUE, 0, 0, 900);
  init_color(COLOR_ORANGE, 255 * 3.9, 128 * 3.9, 0);
  init_color(COLOR_RED, 800, 0, 0);
  init_color(COLOR_PURPLE, 900, 0, 900);
  init_color(COLOR_YELLOW, 900, 900, 0);
  init_color(COLOR_GREEN, 0, 900, 0);
  init_color(COLOR_BLUE, 0, 900, 900);

  init_pair(9, COLOR_DARKBLUE, COLOR_DARKBLUE);
  init_pair(1, COLOR_ORANGE, COLOR_ORANGE);
  init_pair(2, COLOR_RED, COLOR_RED);
  init_pair(3, COLOR_PURPLE, COLOR_PURPLE);
  init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(5, COLOR_GREEN, COLOR_GREEN);
  init_pair(6, COLOR_BLUE, COLOR_BLUE);

  init_pair(WALL, COLOR_WHITE,
            COLOR_WHITE);  // первое цвет текста, второе цвет фона текста

  init_color(COLOR_DARK_BLACK, 0, 0, 0);
  init_color(COLOR_LIGHT_YELLOW, 255 * 3.9, 178 * 3.9, 102 * 3.9);

  init_pair(COLOR_DARK_BLACK, COLOR_DARK_BLACK, COLOR_DARK_BLACK);
  init_pair(COLOR_LIGHT_YELLOW, COLOR_LIGHT_YELLOW, COLOR_LIGHT_YELLOW);
  init_pair(14, COLOR_RED, COLOR_BLACK);
}

void printTetrisIntro() {
  int a[15][15] = {
      {8, 8, 8, 8, 8, 12, 12, 12, 12, 12, 8, 8, 8, 8, 8},
      {8, 8, 8, 12, 12, 13, 13, 13, 4, 13, 12, 12, 8, 8, 8},
      {8, 8, 12, 1, 13, 13, 4, 4, 4, 4, 4, 1, 12, 8, 8},
      {8, 12, 1, 13, 13, 4, 4, 7, 7, 7, 4, 4, 13, 12, 8},
      {8, 12, 13, 13, 12, 12, 4, 7, 7, 12, 12, 4, 4, 12, 8},
      {12, 1, 13, 12, 12, 12, 12, 7, 12, 12, 12, 12, 4, 1, 12},
      {12, 13, 13, 12, 12, 2, 12, 4, 12, 12, 2, 12, 4, 4, 12},
      {12, 1, 13, 13, 12, 12, 4, 4, 4, 12, 12, 7, 4, 13, 12},
      {12, 2, 1, 13, 13, 13, 13, 13, 4, 4, 4, 4, 13, 13, 12},
      {12, 2, 1, 12, 1, 13, 13, 13, 13, 13, 13, 12, 13, 1, 12},
      {8, 12, 2, 1, 12, 12, 12, 12, 12, 12, 12, 13, 13, 12, 8},
      {8, 12, 2, 1, 1, 12, 12, 12, 12, 12, 13, 13, 1, 12, 8},
      {8, 8, 12, 2, 1, 1, 12, 12, 12, 13, 13, 1, 12, 8, 8},
      {8, 8, 8, 12, 12, 2, 2, 1, 1, 1, 12, 12, 8, 8, 8},
      {8, 8, 8, 8, 8, 12, 12, 12, 12, 12, 8, 8, 8, 8, 8},
  };

  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {
      attron(COLOR_PAIR(a[i][j]));
      mvprintw(i + 2, (j + 2) * 2, "%d%d", a[i][j], a[i][j]);
    }
  }

  mvvline(7, 34, '@', 5);
  mvvline(7, 35, '@', 5);
  attrset(A_NORMAL);
  attron(A_BOLD);
  mvprintw(12, 40, "WANNA PLAY???");
  attrset(A_NORMAL);
  attron(COLOR_PAIR(14));
  attron(A_BLINK);
  mvprintw(14, 38, "Press \"Enter\"....");
  attrset(A_NORMAL);
}

void clearWindow() {
  for (int i = 0; i < ROWS; i++) {
    mvhline(i, 0, ' ', 90);
  }
}

/**
 * @brief init game after press "Enter"
 * @param[out] game game current status
 * @param[out] var variables of current object
 * @param[in] objects list of objects
 * @param[in] numberObject
 * @details
 * preparing the window to start the game
 */
void firstInit(GameInfo_t *game, DynamicObject *var, int (*objects)[5][5]) {
  clearWindow();
  srand(time(NULL));
  var->nextNumberObject = (rand() % 7) * 4;
  changeObject(var);
  fillField(game);
  printField(game);

  game->pause = 1;
  game->level = 1;
  game->speed = 110;
  game->score = 0;

  printObject(objects, 17, 9, var->nextNumberObject);
  printFrame();
  printGameKeys();
}