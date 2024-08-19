/// \file

#include <stdlib.h>
#include <time.h>

#include "brick.h"

/**
 * @brief save object in game field
 * @param[in] objects list of objects
 * @param[in,out] var variables of current object
 * @param[out] game game information
 */
void saveObject(int (*objects)[5][5], DynamicObject *var, GameInfo_t *game) {
  var->y--;
  var->x--;
  for (int i = var->y, k = 0; i < var->y + 5; i++, k++) {
    for (int j = var->x, c = 0; j < var->x + 5; j++, c++) {
      if (objects[var->numberObject][k][c] != SPACE) {
        game->field[i][j] = objects[var->numberObject][k][c];
      }
    }
  }
}

/**
 * @brief change current object to next
 * @param[in,out] var variables of current object
 * @details
 * changes the object by changing the number and
 * determines the number of the next object
 */
void changeObject(DynamicObject *var) {
  var->y = 1;
  var->x = (COLUMS / 2) - 2;
  var->numberObject = var->nextNumberObject;
  var->countRotation = 0;
  while (var->nextNumberObject == var->numberObject)
    var->nextNumberObject = (rand() % 7) * 4;
}

/**
 * @brief move current object
 * @param[in] objects list of objects
 * @param[in,out] var variables of current object
 * @param[in] add_X possible addition to the var->x variable
 * @param[in] add_Y possible addition to the var->y variable
 * @details
 * moving object by changing the coordinates if the borders allow.
 * @return OK if borders allow, FAIL_TO_MOVE if not.
 */
int moveObject(int (*objects)[5][5], DynamicObject *var, int add_X, int add_Y) {
  int return_value = checkBorders(objects, var, add_X, add_Y);

  if (return_value == OK) {
    var->x += add_X;
    var->y += add_Y;
  }

  return return_value;
}

/**
 * @brief check borders of moving objectx`
 * @param[in] objects list of objects
 * @param[in] var variables of current object
 * @param[in] add_X possible addition to the var->x variable
 * @param[in] add_Y possible addition to the var->y variable
 * @details
 * check if the borders allow
 * @return OK if borders allow, FAIL_TO_MOVE if not.
 */
int checkBorders(int (*objects)[5][5], DynamicObject *var, int add_X,
                 int add_Y) {
  int return_value = OK;
  GameInfo_t *game = updateCurrentState();

  int new_Y = var->y + add_Y;
  int new_X = var->x + add_X;

  for (int i = new_Y - 1, k = 0; (i < new_Y + 4) && return_value == OK;
       i++, k++) {
    for (int j = new_X - 1, c = 0; (j < new_X + 4) && return_value == OK;
         j++, c++) {
      if (i >= ROWS) continue;
      if ((game->field[i][j] != SPACE) &&
          objects[var->numberObject][k][c] != SPACE)
        return_value = FAIL_TO_MOVE;
    }
  }

  return return_value;
}

/**
 * @brief count the game score
 * @param [in,out] game game current information
 * @details
 * change game->score, game->high_score, game->level
 */
void countThePoints(GameInfo_t *game) {
  int countDeletedRows = dropField(game);
  switch (countDeletedRows) {
    case 1:
      game->score += 100;
      break;

    case 2:
      game->score += 300;
      break;

    case 3:
      game->score += 700;
      break;

    case 4:
      game->score += 1500;
      break;
  }
  if (game->score > game->high_score) game->high_score = game->score;
  game->level = (game->score >= 6000) ? 10 : 1 + (game->score / 600);
  if (game->level != 1) game->speed = 110 - (game->level * GAME_DIFICULTY);
}

/**
 * @brief drop current field
 * @param[in,out] game game current information
 * @details
 * checks if the entire line is filled in. If it is, function count the rows
 * that should be destroy and drop field in their place.
 * @return count the rows that destroys
 */
int dropField(GameInfo_t *game) {
  int countDeletedRows = 0;
  int blocks = 0;
  for (int i = 1; i < ROWS; i++) {
    blocks = 0;
    for (int j = 1; j < COLUMS; j++) {
      if (game->field[i][j] != SPACE && game->field[i][j] != WALL) blocks++;
    }

    if (blocks == COLUMS - 2) {
      countDeletedRows++;
      // Смещение всех строк вниз, начиная с текущей строки
      for (int k = i; k > 0; k--) {
        for (int q = 1; q < COLUMS; q++) {
          game->field[k][q] = game->field[k - 1][q];
        }
      }
    }
  }

  return countDeletedRows;
}

/**
 * @brief fill field game
 * @param[out] game game current information
 * @details
 * fill by numbers defined denoting an empty space or walls
 */
void fillField(GameInfo_t *game) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMS; j++) {
      if ((j + 1) == COLUMS || j == 0) {
        game->field[i][j] = WALL;
      } else if ((i + 1) == ROWS) {
        game->field[i][j] = WALL;
      } else {
        game->field[i][j] = SPACE;
      }
    }
  }
}

/**
 * @brief get current object variables
 * @return a pointer to a static structure
 */
DynamicObject *getVariables() {
  static DynamicObject variables = {0};
  return &variables;
}

/**
 * @brief get other variables
 * @return a pointer to a static structure
 */
OtherVariables *getOtherVar() {
  static OtherVariables Ovariables = {0};
  return &Ovariables;
}

/**
 * @brief get current game status
 * @return a pointer to a static structure
 */
GameInfo_t *updateCurrentState() {
  static GameInfo_t game = {0};
  return &game;
}
