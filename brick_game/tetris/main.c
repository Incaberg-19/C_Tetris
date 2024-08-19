/// \file
#include <ncurses.h>

#include "brick.h"

int main(void) {
  GameInfo_t *tetris = updateCurrentState();

  if (allocateMatrixMemory(&(tetris->field)) == OK) {
    DynamicObject *var = getVariables();
    int(*objects)[5][5] = listObjects();

    initscr();  // Запускает режим ncurses
    keypad(stdscr, TRUE);  // Включает специальные клавиши клавиатуры
    noecho();  // Отключает отображение ввода пользователя на экране
    curs_set(FALSE);  // Скрывает курсоp

    initCollors();
    gameLoop(objects, var, tetris);

    freeMatrixMemory(tetris->field, 0);
    endwin();
  }

  return 0;
}

/**
 * @brief game loop
 * @param[in,out] objects list of object
 * @param[in,out] var variables of current object
 * @param[out] game game current information
 * @details
 * The main cycle of the game.
 * Here commands are received from the user, the screen is updated,
 * and related functions are called.
 */
void gameLoop(int (*objects)[5][5], DynamicObject *var, GameInfo_t *game) {
  OtherVariables *othervar = getOtherVar();
  printTetrisIntro(othervar);
  readHighScore(&game->high_score);
  do {
    othervar->command = getch();
    controls(othervar, game);
    if (game->pause && othervar->start) {
      if (checkBorders(objects, var, 0, 0) != OK) {
        printField(game);
        userInput(Lose, 0);
      } else {
        printField(game);
        printObject(objects, var->x, var->y, var->numberObject);
        refresh();
      }
    }
  } while (othervar->command != QUIT);
}