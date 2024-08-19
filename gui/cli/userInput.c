/// \file
#include <ncurses.h>

#include "../../brick_game/tetris/brick.h"

/**
 * @brief redirection keyboard keys
 * @param[in] game current game status
 * @param[in,out] othervar other variables
 * @details
 * redirection keyboard keys to userInput, check if game is pause/started,
 * change time when is time
 */
void controls(OtherVariables *othervar, GameInfo_t *game) {
  switch (othervar->command) {
    case ' ':
      userInput(Pause, 0);
      break;
    case 'q':
      userInput(Terminate, 0);
      break;
    case '\n':
      if (!othervar->start) userInput(Start, 0);
      break;
  }

  if (game->pause && othervar->start) {
    (othervar->timer > 10) ? (othervar->timer = 0, userInput(Down, 0))
                           : othervar->timer++;

    switch (othervar->command) {
      case 'r':
        userInput(Action, 0);
        break;
      case KEY_LEFT:
        userInput(Left, 0);
        break;
      case KEY_RIGHT:
        userInput(Right, 0);
        break;
      case KEY_DOWN:
        userInput(Down, 0);
        break;
      case KEY_SF:
        userInput(Down, 1);
        break;
    }
  }
}

/**
 * @brief process user input
 * @param[in] action action from user
 * @param[in] hold is button pressed
 * @details
 * selects which function to use with which key pressed,
 * change game settings if game start, lose, pause
 */
void userInput(UserAction_t action, bool hold) {
  DynamicObject *var = getVariables();
  int(*objects)[5][5] = listObjects();
  GameInfo_t *game = updateCurrentState();
  OtherVariables *othervar = getOtherVar();
  int temp = 0, tempCounter = 0;
  switch (action) {
    case Action:
      temp = var->numberObject;
      tempCounter = var->countRotation;
      (var->countRotation != 3)
          ? (var->numberObject++, var->countRotation++)
          : (var->numberObject -= 3, var->countRotation = 0);
      if (moveObject(objects, var, 0, 0) != OK) {
        var->numberObject = temp;
        var->countRotation = tempCounter;
      }
      break;
    case Left:
      moveObject(objects, var, -1, 0);
      break;
    case Right:
      moveObject(objects, var, 1, 0);
      break;
    case Down:
      if (hold == 0) {
        if (moveObject(objects, var, 0, 1) != OK) {
          processDown(objects, var, game, othervar);
        }
      } else {
        while (moveObject(objects, var, 0, 1) != FAIL_TO_MOVE)
          ;
        processDown(objects, var, game, othervar);
      }
      break;
    case Pause:
      game->pause = !game->pause;
      break;
    case Terminate:
      othervar->command = QUIT;
      saveHighScore(&game->high_score);
      break;
    case Start:
      othervar->start = 1;
      processStartLose(game, var, othervar, objects);
      break;
    case Lose:
      othervar->start = 0;
      processStartLose(game, var, othervar, objects);
      break;
  }
}

/**
 * @brief collision lower bound process
 * @param[in,out] objects list of object
 * @param[in,out] var variables of current object
 * @param[out] othervar other variables to use
 * @param[out] game game current information
 * @details handling a collision with a lower bound
 */
void processDown(int (*objects)[5][5], DynamicObject *var, GameInfo_t *game,
                 OtherVariables *othervar) {
  saveObject(objects, var, game);
  countThePoints(game);
  timeout(game->speed);
  changeObject(var);
  printObject(objects, 17, 9, var->nextNumberObject);
  attrset(A_NORMAL);
  attron(A_BOLD);
  mvprintw(3, 29, "%d", game->score);
  mvprintw(5, 29, "%d", game->high_score);
  mvprintw(15, 38, "%d", game->level);
  attroff(A_BOLD);
  othervar->timer = 0;
}

/**
 * @brief process restart game
 * @param[in,out] objects list of object
 * @param[in,out] var variables of current object
 * @param[out] othervar other variables to use
 * @param[out] game game current information
 * @details call the needed function when its start/lose
 */
void processStartLose(GameInfo_t *game, DynamicObject *var,
                      OtherVariables *othervar, int (*objects)[5][5]) {
  // 1 - start, 2 - Lose
  if (othervar->start == 1) {
    firstInit(game, var, objects);
    attron(A_BOLD);
    mvprintw(3, 29, "%d", game->score);
    mvprintw(5, 29, "%d", game->high_score);
    mvprintw(15, 38, "%d", game->level);
    attroff(A_BOLD);
    timeout(game->speed);
  } else {
    attron(A_BLINK);
    attron(A_BOLD);
    mvprintw(15, 52, "U lose, what a pity :(");
    mvprintw(17, 52, "Press \"Enter\" to try again");
    attrset(A_NORMAL);
  }
}