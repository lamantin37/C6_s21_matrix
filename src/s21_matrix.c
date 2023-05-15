#include "s21_matrix.h"

// int main() {
//   int res;
//   matrix_t A = {0};
//   matrix_t Z = {0};

//   s21_create_matrix(1, 1, &A);
//   A.matrix[0][0] = 2;

//   res = s21_inverse_matrix(&A, &Z);

//   s21_remove_matrix(&A);
//   s21_remove_matrix(&Z);
//   return 0;
// }

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int return_value = 0;
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
          return_value = 1;
          break;
        }
      }
    } else {
      result->rows = 0;
      result->columns = 0;
      return_value = 1;
    }
    if (return_value == 0) {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
          result->matrix[i][j] = 0.0;
        }
      }
    }
  } else {
    return_value = 1;
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

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int return_value = 1;
  if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0 ||
      !A || !B || A->rows != B->rows || A->columns != B->columns) {
    return_value = 0;
  }

  if (return_value == 1) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) { // fix: use j instead of i
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > DIFF_LIMIT) {
          return_value = 0;
          break;
        }
      }
      if (return_value == 0) {
        break;
      }
    }
  }

  return return_value;
}

///////////////////////////////////////////////////////////////////

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int return_value = A->rows <= 0 || A->columns <= 0 || B->rows <= 0 ||
                     B->columns <= 0 || !A || !B || !result;

  if (A->rows != B->rows || A->columns != B->columns) {
    return_value = 2;
  }

  if (return_value == 0) {
    if (s21_create_matrix(A->rows, A->columns, result) != 0) {
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
  int return_value = 0;
  if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0 ||
      !A || !B || !result) {
    return_value = 1;
  }

  if (A->rows != B->rows || A->columns != B->columns) {
    return_value = 2;
  }

  if (return_value == 0) {
    if (s21_create_matrix(A->rows, A->columns, result) != 0) {
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
    if (s21_create_matrix(A->rows, A->columns, result) != 0) {
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
  int return_value = 0;

  if (A->rows <= 0 || A->columns <= 0 || B->rows <= 0 || B->columns <= 0 ||
      !A || !B || !result) {
    return_value = 1;
  }
  if (A->columns != B->rows) {
    return_value = 2;
  }

  if (return_value == 0) {
    if (s21_create_matrix(A->rows, B->columns, result) != 0) {
      return_value = 2;
    }
  }

  if (return_value == 0) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        for (int z = 0; z < B->rows; z++)
          result->matrix[i][j] += A->matrix[i][z] * B->matrix[z][j];
      }
    }
  }

  return return_value;
}

///////////////////////////////////////////////////////////////////

int s21_transpose(matrix_t *A, matrix_t *result) {
  int return_value = !A || !result || A->rows <= 0 || A->columns <= 0;

  if (return_value == 0) {
    if (s21_create_matrix(A->columns, A->rows, result) != 0) {
      return_value = 2;
    }
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }

  return return_value;
}

int s21_determinant(matrix_t *A, double *result) {
  int return_value =
      !A || !result || !A->matrix || A->rows <= 0 || A->columns <= 0;
  int one_flag = 0;

  if (return_value == 0) {
    if (A->rows != A->columns) {
      return_value = 2;
    }
    if (return_value == 0) {
      if (A->rows == 1 && A->columns) {
        *result = A->matrix[0][0];
        one_flag = 1;
      }
      if (!one_flag) {
        double det = 0;
        int sign = 1;
        matrix_t minor;

        for (int j = 0; j < A->columns; j++) {
          s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
          s21_get_minor(A, 0, j, &minor);
          double minor_det = 0;
          s21_determinant(&minor, &minor_det);
          det += sign * A->matrix[0][j] * minor_det;
          sign *= -1;
          s21_remove_matrix(&minor);
        }

        *result = det;
      }
    }
  }
  return return_value;
}

int s21_get_minor(matrix_t *A, int row, int col, matrix_t *result) {
  int i_result = 0;
  int j_result;
  for (int i = 0; i < A->rows; i++) {
    if (i == row) {
      continue;
    }
    j_result = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j == col) {
        continue;
      }
      result->matrix[i_result][j_result] = A->matrix[i][j];
      j_result++;
    }
    i_result++;
  }
  return 0;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int return_value = !A || !result || !A->matrix;
  int one_flag = 0;
  if (return_value == 0) {
    if (A->rows != A->columns || A->rows <= 0 || A->columns <= 0) {
      return_value = 2;
    }
    if (return_value == 0) {
      s21_create_matrix(A->rows, A->columns, result);
      if (A->rows == 1 && A->columns == 1) {
        double det = 0.f;
        s21_determinant(A, &det);
        result->matrix[0][0] = (1 / det) * A->matrix[0][0];
        one_flag = 1;
      }
      if (one_flag == 0) {
        matrix_t minor;
        int sign;
        for (int i = 0; i < A->rows; i++) {
          for (int j = 0; j < A->columns; j++) {
            s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
            s21_get_minor(A, i, j, &minor);
            double det = 0.f;
            s21_determinant(&minor, &det);
            result->matrix[i][j] = pow(-1, i + j) * det;
            s21_remove_matrix(&minor);
          }
        }
      }
    }
  }
  return return_value;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int return_value = !A || !result || !A->matrix;
  if (return_value == 0) {
    if (A->rows != A->columns || A->rows <= 0 || A->columns <= 0) {
      return_value = 2;
    } else {
      double det;
      int det_code = 0;
      if ((det_code = s21_determinant(A, &det)) != 0 || det == 0) {
        return_value = 2;
      } else {
        matrix_t com;
        int com_code = 0;
        if ((com_code = s21_calc_complements(A, &com)) != 0) {
          return_value = com_code;
        } else {
          matrix_t com_t;
          int com_t_code = 0;
          if ((com_t_code = s21_transpose(&com, &com_t)) != 0) {
            s21_remove_matrix(&com);
            return_value = com_t_code;
          } else {
            s21_remove_matrix(&com);
            double inv_det = 1 / det;
            s21_mult_number(&com_t, inv_det, result);

            s21_remove_matrix(&com_t);
          }
        }
      }
    }
  }
  return return_value;
}