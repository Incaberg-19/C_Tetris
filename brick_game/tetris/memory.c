/// \file
#include <stdlib.h>

#include "brick.h"

/**
 * @brief allocate matrix memory
 * @param[out] matrix
 * @return OK if allocate was success, FAIL_TO_ALLOCATE if its not
 */
int allocateMatrixMemory(int ***matrix) {
  int return_value = OK;
  *matrix = malloc(ROWS * sizeof(int *));
  if (*matrix != NULL) {
    for (int i = 0; i < ROWS && return_value == OK; i++) {
      (*matrix)[i] = malloc(COLUMS * sizeof(int));
      if ((*matrix)[i] == NULL) {
        freeMatrixMemory(*matrix, i);
        return_value = FAIL_TO_ALLOCATE;
      }
    }
  } else {
    return_value = FAIL_TO_ALLOCATE;
  }
  return return_value;
}

/**
 * @brief free allocated matrix memory
 * @param[out] matrix
 * @param[in] i to which line to free up memory
 */
void freeMatrixMemory(int **matrix, int i) {
  if (i == 0) i = ROWS;
  for (int j = 0; j < i; j++) {
    free(matrix[j]);
  }
  free(matrix);
}
