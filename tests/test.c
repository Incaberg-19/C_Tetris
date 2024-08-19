#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../brick_game/tetris/brick.h"

#define True 1
#define False 0

int countDetailsOfObjects(GameInfo_t *game, int str) {
  int counter = 0;
  for (int i = str; i < ROWS; i++) {
    for (int j = 0; j < COLUMS; j++) {
      if (game->field[i][j] != 7 && game->field[i][j] != 8) counter++;
    }
  }
  return counter;
}

void printField(GameInfo_t *game) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMS; j++) {
      printf("%d ", game->field[i][j]);
    }
    printf("\n");
  }
}

START_TEST(test_memory_functions) {
  GameInfo_t *game = updateCurrentState();
  // DynamicObject *variables=getVariables();
  // int (*objects)[5][5] = listObjects();
  // OtherVariables *var=getOtherVar();
  int check = allocateMatrixMemory(&(game->field));
  ck_assert_int_eq(check, OK);
  freeMatrixMemory(game->field, 0);

  int **a = malloc(5 * sizeof(int *));
  for (int i = 0; i < 5; i++) {
    a[i] = malloc(2 * sizeof(int));
  }
  freeMatrixMemory(a, 5);
}
END_TEST

START_TEST(test_fill_field) {
  GameInfo_t *game = updateCurrentState();
  // DynamicObject *variables=getVariables();
  // int (*objects)[5][5] = listObjects();
  // OtherVariables *var=getOtherVar();
  allocateMatrixMemory(&(game->field));
  fillField(game);
  int check = True;

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMS; j++) {
      if (((j + 1) == COLUMS || j == 0) && game->field[i][j] != WALL) {
        check = False;
      }
      if ((i + 1) == ROWS && game->field[i][j] != WALL) {
        check = False;
      }
    }
  }

  ck_assert_int_eq(check, True);

  freeMatrixMemory(game->field, 0);
}
END_TEST

START_TEST(test_objects_functions) {
  GameInfo_t *game = updateCurrentState();
  DynamicObject *var = getVariables();
  int(*objects)[5][5] = listObjects();
  // OtherVariables *othervar=getOtherVar();
  allocateMatrixMemory(&(game->field));
  fillField(game);

  srand(time(NULL));
  var->nextNumberObject = (rand() % 7) * 4;
  game->pause = 1;
  game->level = 1;
  game->speed = 110;
  game->score = 0;

  changeObject(var);
  saveObject(objects, var, game);
  int check = False;
  for (int i = var->y, k = 0; i < var->y + 5; i++, k++) {
    for (int j = var->x, c = 0; j < var->x + 5; j++, c++) {
      if (objects[var->numberObject][k][c] != SPACE) {
        check = True;
      }
    }
  }
  ck_assert_int_eq(check, True);

  int saveNumber = var->numberObject, count1 = countDetailsOfObjects(game, 0),
      count2 = 0;
  fillField(game);
  changeObject(var);
  var->numberObject = saveNumber;
  while (moveObject(objects, var, 0, 1) != FAIL_TO_MOVE)
    ;
  saveObject(objects, var, game);
  count2 = countDetailsOfObjects(game, 10);
  ck_assert_int_eq(count1, count2);
  freeMatrixMemory(game->field, 0);
}
END_TEST

void specialFillTest(GameInfo_t *game, int q) {
  for (int i = q; i < ROWS; i++) {
    for (int j = 0; j < COLUMS; j++) {
      if ((j + 1) == COLUMS || j == 0) {
        game->field[i][j] = WALL;
      } else if ((i + 1) == ROWS) {
        game->field[i][j] = WALL;
      } else {
        game->field[i][j] = 9;
      }
    }
  }
}

START_TEST(test_countPoins_and_dropField) {
  GameInfo_t *game = updateCurrentState();
  // DynamicObject *variables=getVariables();
  // int (*objects)[5][5] = listObjects();
  // OtherVariables *var=getOtherVar();
  allocateMatrixMemory(&(game->field));
  fillField(game);
  specialFillTest(game, 16);
  countThePoints(game);
  ck_assert_int_eq(game->score, 1500);
  ck_assert_int_eq(game->score, game->high_score);
  int counter = countDetailsOfObjects(game, 0);
  ck_assert_int_eq(counter, 0);
  ck_assert_int_eq(game->level, 3);

  specialFillTest(game, 17);
  countThePoints(game);

  specialFillTest(game, 18);
  countThePoints(game);

  specialFillTest(game, 19);
  countThePoints(game);

  specialFillTest(game, 16);
  countThePoints(game);

  specialFillTest(game, 16);
  countThePoints(game);

  specialFillTest(game, 16);
  countThePoints(game);

  ck_assert_int_eq(game->level, 10);
  readHighScore(&game->score);
  saveHighScore(&game->high_score);
  readHighScore(&game->score);

  freeMatrixMemory(game->field, 0);
}
END_TEST

void case_test(Suite *s, int *fail) {
  SRunner *runner = srunner_create(s);
  srunner_run_all(runner, CK_NORMAL);
  srunner_set_fork_status(runner, CK_NOFORK);
  *fail = srunner_ntests_failed(runner);
  srunner_free(runner);
}

Suite *s21_brick_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_brick");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_memory_functions);
  tcase_add_test(tc_core, test_fill_field);
  tcase_add_test(tc_core, test_objects_functions);
  tcase_add_test(tc_core, test_countPoins_and_dropField);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int no_falied = 0;

  case_test(s21_brick_suite(), &no_falied);
  return !no_falied ? 0 : 1;
}