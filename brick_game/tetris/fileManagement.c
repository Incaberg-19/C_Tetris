/// \file
#include <stdio.h>

#include "brick.h"

/**
 * @brief save high score in file
 * @param[in] highScore
 * @details
 * overwriting score file
 */
void saveHighScore(int *highScore) {
  FILE *file = fopen("score.txt", "w");
  if (file == NULL) {
    fprintf(stderr, "Fail to create file\n");
  } else {
    fprintf(file, "HIGHSCORE:\n%d", *highScore);
    fclose(file);
  }
}

/**
 * @brief read high score from file
 * @param[out] highScore
 * @details
 * if file does not exist the function create a new file
 */
void readHighScore(int *highScore) {
  FILE *file = fopen("score.txt", "r");
  if (file == NULL) {
    saveHighScore(highScore);
  } else {
    while (fgetc(file) != '\n')
      ;
    fscanf(file, "%d", highScore);
    fclose(file);
  }
}