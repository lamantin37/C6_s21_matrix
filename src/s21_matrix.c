#include "s21_matrix.h"

int main() {
  matrix_t matrix1;
  matrix_t matrix2;
  s21_create_matrix(3, 2, &matrix1);
  s21_create_matrix(3, 2, &matrix2);
  // matrix2.matrix[2][1] = 7.1;
  printf("%d\n", s21_eq_matrix(&matrix1, &matrix2));
  s21_remove_matrix(&matrix1);
  s21_remove_matrix(&matrix2);
  return 0;
}

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int return_value = SUCCESS;
  if (result && rows > 0 && columns > 0) {
    result->rows = rows;
    result->columns = columns;
    if (result->matrix = (double **)malloc(rows * sizeof(double *))) {
      for (int i = 0; i < rows; i++) {
        if (!(result->matrix[i] = (double *)malloc(columns * sizeof(double)))) {
          for (int j = 0; j < i; j++) {
            free(result->matrix[j]);
          }
          free(result->matrix);
          result->rows = 0;
          result->columns = 0;
          return_value = FAILURE;
          break;
        }
      }
    } else {
      result->rows = 0;
      result->columns = 0;
      return_value = FAILURE;
    }
    if (return_value == SUCCESS) {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
          result->matrix[i][j] = 0.0;
        }
      }
    }
  } else {
    return_value = FAILURE;
  }

  return return_value;
}

void s21_remove_matrix(matrix_t *A) {
  if (A) {
    for (int i = 0; i != A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->columns = 0;
    A->rows = 0;
  }
}

///////////////////////////////////////////////////////////////////

int check_incorrect_matrix(matrix_t *A, matrix_t *B) {
  return (A->rows != B->rows || A->columns != B->columns || A->rows <= 0 ||
          B->rows <= 0 || A->columns <= 0 || B->columns <= 0);
}

///////////////////////////////////////////////////////////////////

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int return_value = check_incorrect_matrix(A, B) ? FAILURE : SUCCESS;

  if (return_value == SUCCESS) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) { // fix: use j instead of i
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > DIFF_LIMIT) {
          return_value = FAILURE;
          break;
        }
      }
      if (return_value == FAILURE) {
        break;
      }
    }
  }

  return return_value;
}

///////////////////////////////////////////////////////////////////

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int return_value = check_incorrect_matrix(A, B) || !A || !B || !result;

  if (return_value == 0) {
    if (s21_create_matrix(A->rows, A->columns, result) != SUCCESS) {
      return_value = 2;
    }
  }

  if (return_value == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }

  return return_value;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int return_value = check_incorrect_matrix(A, B) || !A || !B || !result;

  if (return_value == 0) {
    if (s21_create_matrix(A->rows, A->columns, result) != SUCCESS) {
      return_value = 2;
    }
  }

  if (return_value == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }

  return return_value;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int return_value = !A || !result || A->rows <= 0 || A->columns <= 0;

  if (return_value == 0) {
    if (s21_create_matrix(A->rows, A->columns, result) != SUCCESS) {
      return_value = 2;
    }
  }

  if (return_value == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }

  return return_value;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int return_value = check_incorrect_matrix(A, B) || !A || !B || !result;

  if (return_value == 0) {
    if (s21_create_matrix(A->rows, A->columns, result) != SUCCESS) {
      return_value = 2;
    }
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      int sum = 0;
      for (int k = 0; k < A->columns; k++) {
        sum += A->matrix[i][k] * B->matrix[k][j];
      }
      result->matrix[i][j] = sum;
    }
  }

  return return_value;
}

///////////////////////////////////////////////////////////////////

int s21_transpose(matrix_t *A, matrix_t *result) {
  int return_value = !A || !result || A->rows <= 0 || A->columns <= 0;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }
  result->rows = A->columns;
  result->columns = A->rows;

  return 0;
}