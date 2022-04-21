#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double dotProduct(double *, double *, size_t);
double **matMul(double **, double**, size_t, size_t, size_t, size_t);
void printMat(double**, size_t, size_t);
double *inputFloatList(char *, size_t *);
double **userMat(size_t*, size_t*);
void freeMat(double **, size_t);


int main(void) {

    size_t A_rows, A_cols, B_rows, B_cols;

    double **A = userMat(&A_rows, &A_cols);
    puts("A:");
    printMat(A, A_rows, A_cols);

    double **B = userMat(&B_rows, &B_cols);
    puts("B:");
    printMat(B, B_rows, B_cols);

    double **C = matMul(A, B, A_rows, A_cols, B_rows, B_cols);
    if (C) {
        puts("A * B:");
        printMat(C, A_rows, B_cols);
        freeMat(C, A_rows);
    }
    freeMat(A, A_rows);
    freeMat(B, B_rows);

    return 0;
}

// Multiplies 2 matrices. Remember that the resulting vector is of A_rows and B_cols.
double **matMul(double **A, double **B, size_t A_rows, size_t A_cols, size_t B_rows, size_t B_cols)
{
    if (A_rows != B_cols) { // return nothing if the matrices are inadequate for multiplication..
        puts("Dimension error!");
        return NULL;
    }
    // Create new matrix of A_rows and B cols.
    double **C = malloc(A_rows * sizeof(double*));
    for (int i = 0; i < A_rows; i++)
        C[i] = calloc(B_cols, sizeof(double)); // initialized for sum

    for (int i = 0; i < A_rows; i++) {
        for (int j = 0; j < B_cols; j++) {
            for(int k = 0; k < B_cols; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

// Prints a given matrix to the screen.
void printMat(double **A, size_t Ar, size_t Ac)
{
    for (int i = 0; i < Ar; i++) {
        printf("[");
        for (int j = 0; j < Ac; j++) {
            printf("%.2lf%s", A[i][j], j + 1 < Ac ? "\t" : "]\n");
        }
    }
}

// Reads an input list of floats.
double *inputFloatList(char *prompt, size_t *len)
{
    char buf[120];
    char *captured;
    double f_buf[120], *ar;

    *len = 0;

    printf(prompt); // Prompt user for input.
    fgets(buf, sizeof(buf), stdin); // Takes in the string.

    if (strlen(buf) > 1) {

        // Parse input to floats.
        do {
            captured = strtok((*len) > 0 ? NULL : buf, " ");

            if (captured != NULL && strcmp(captured, "\n") != 0) {
                f_buf[(*len)] = atof(captured);
                (*len)++;
            }


        } while (captured != NULL);


        // Create heap array and copy floats to it.
        ar = malloc(*len * sizeof(double));
        for (int i = 0; i < *len; i++)
            ar[i] = f_buf[i];
   }


    return ar;
}

// Creates a matrix from user input.
double **userMat(size_t *rows, size_t *cols)
{

    double *matBuf[120], **mat = NULL;
    size_t c_count;
    *rows = 0;
    *cols = 0;

    // Takes user input until user gives an empty row.
    do {
        char prompt[32];

        sprintf(prompt, "row %i: ", *rows);
        matBuf[*rows] = inputFloatList(prompt, &c_count);

        if (*rows > 0 && c_count != *cols && c_count > 0) {
            puts("Inconsistent row!");
            free(matBuf[*rows]); // clears the given row if it doesn't match with the first row's length.
        } else if (*rows > 0 && c_count == *cols) {
            (*rows)++;
        } else if (*rows == 0) {
            *cols = c_count; // set matrix columns as first row's length.
            (*rows)++;
        }


    } while (c_count > 0);

    if ((*rows) > 0 && (*cols) > 0) { // create final matrix.
        mat = malloc((*rows) * sizeof(double*));
        for (int i = 0; i < (*rows); i++) mat[i] = matBuf[i];
    } else {
        free(mat[0]);
    }

    return mat;
}

void freeMat(double **M, size_t rows) {
    for (int i = 0; i < rows; i++) {
        free(M[i]);
    } free(M);
}
