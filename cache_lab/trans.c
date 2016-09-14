/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
void trans(int M, int N, int A[N][M], int B[M][N]);
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
  int i, j, n, m;
  int temp[8];
  
  if (M == 32 && N == 32) {
    for (n = 0; n < N; n +=8)
      for (m = 0; m < M; m += 8)
	for (i = n; i < n + 8; i++) {
	  for (j = m; j < m + 8; j++) {
	    if (i == j)
	      temp[i % 8] = A[i][j];
	    else
	      B[j][i] = A[i][j];
	  }
	  if (n == m) {
	    for (j = n; j < n + 8; j++)
	      B[j][j] = temp[j % 8];
	  }
	} 
  } else if (M == 64 && N == 64) {
    for (n = 0; n < N; n += 8)
      for (m = 0; m < M; m += 8){
	for (i = n; i < n + 4; i++) {
	  for (j = m; j < m + 4; j++) {
	    if (i == j)
	      temp[i % 4] = A[i][j];
	    else
	      B[j][i] = A[i][j];
	  }
	  for (j = m + 4; j < m + 8; j++)
	    B[j - 4][i + 4] = A[i][j];
	}
	if (n == m)
	  for (i = n; i < n + 4; i++)
	    B[i][i] = temp[i % 4];

	for (i = 0; i < 4; i++) {
	  temp[0] = B[m + i][n + 4];
	  temp[1] = B[m + i][n + 5];
	  temp[2] = B[m + i][n + 6];
	  temp[3] = B[m + i][n + 7];

	  B[m + i][n + 4] = A[n + 4][m + i];
	  B[m + i][n + 5] = A[n + 5][m + i];
	  B[m + i][n + 6] = A[n + 6][m + i];
	  B[m + i][n + 7] = A[n + 7][m + i];	  

	  B[m + 4 + i][n] = temp[0];
	  B[m + 4 + i][n + 1] = temp[1];
	  B[m + 4 + i][n + 2] = temp[2];
	  B[m + 4 + i][n + 3] = temp[3];	  
	}

	for (i = n + 4; i < n + 8; i++)
	  for (j = m + 4; j < m + 8; j++){
	    if (i == j)
	      temp[i % 4] = A[i][j];
	    else
	      B[j][i] = A[i][j];
	  }
	if (n == m)
	  for (i = n + 4; i < n + 8; i++)
	    B[i][i] = temp[i % 4];
      }
  } else if (M == 61 && N == 67) {
    for (n = 0; n < 64; n += 8) {

      for (i = n; i < n + 4; i++) {
	for (j = 0; j < 5; j++)
	  temp[j] = A[i][j];
	for (j = 0; j < 5; j++)
	  B[j][i] = temp[j];
      }
      for (i = n + 4; i < n + 8; i++) {
	for (j = 0; j < 5; j++)
	  temp[j] = A[i][j];
	for (j = 0; j < 5; j++)
	  B[j][i] = temp[j];
      }
      
      for (m = 5; m < 61; m += 8){
	for (i = n; i < n + 4; i++) {
	  for (j = m; j < m + 4; j++)
	    temp[j - m] = A[i][j];
	  for (j = m + 4; j < m + 8; j++)
	    temp[j - m] = A[i][j];
	  for (j = m; j < m + 4; j++)
	    B[j][i] = temp[j - m];
	  for (j = m + 4; j < m + 8; j++)
	    B[j - 4][i + 4] = temp[j - m];
	}

	for (i = 0; i < 4; i++) {
	  temp[0] = B[m + i][n + 4];
	  temp[1] = B[m + i][n + 5];
	  temp[2] = B[m + i][n + 6];
	  temp[3] = B[m + i][n + 7];
	  
	  B[m + i][n + 4] = A[n + 4][m + i];
	  B[m + i][n + 5] = A[n + 5][m + i];
	  B[m + i][n + 6] = A[n + 6][m + i];
	  B[m + i][n + 7] = A[n + 7][m + i];
	  
	  B[m + 4 + i][n] = temp[0];
	  B[m + 4 + i][n + 1] = temp[1];
	  B[m + 4 + i][n + 2] = temp[2];
	  B[m + 4 + i][n + 3] = temp[3];	  
	}

	for (i = n + 4; i < n + 8; i++) {
	  for (j = m + 4; j < m + 8; j++)
	    temp[j - m] = A[i][j];
	  for (j = m + 4; j < m + 8; j++)
	    B[j][i] = temp[j - m];
	}
      }
      
    }

    temp[0] = A[64][60];
    temp[1] = A[65][60];
    temp[2] = A[66][60];
    B[60][64] = temp[0]; 
    B[60][65] = temp[1];
    B[60][66] = temp[2];

    for (j = 59; j >= 0; j--)
      for (i = 64; i < 67; i++){
	temp[0] = A[i][j];
	B[j][i] = temp[0];
      }
    
  } else
    trans(M, N, A, B);
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

