#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100

typedef struct {
    int rows;
    int cols;
    int data[MAX_SIZE][MAX_SIZE];
} Matrix;

//thread structure
typedef struct {
    Matrix* matrix;
    int start;
    int end;
    int* sum;
} ThreadArgs;

//swap two integers
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//Quick Sort
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Recursive function to perform Quick Sort
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

//sort a row in ascending order
void* sortRowAscending(void* arguments) {
    ThreadArgs* args = (ThreadArgs*) arguments;

    for (int i = args->start; i <= args->end; i++) {
        quickSort(args->matrix->data[i], 0, args->matrix->cols - 1);
    }

    int rowSum = 0;
    for (int i = args->start; i <= args->end; i++) {
        for (int j = 0; j < args->matrix->cols; j++) {
            rowSum += args->matrix->data[i][j];
        }
    }
    __sync_fetch_and_add(args->sum, rowSum);

    pthread_exit(NULL);
}

// sort a row in descending order
void* sortRowDescending(void* arguments) {
    ThreadArgs* args = (ThreadArgs*) arguments;

    for (int i = args->start; i <= args->end; i++) {
        quickSort(args->matrix->data[i], 0, args->matrix->cols - 1);

        for (int j = 0; j < args->matrix->cols / 2; j++) {
            swap(&args->matrix->data[i][j], &args->matrix->data[i][args->matrix->cols - 1 - j]);
        }
    }

    int rowSum = 0;
    for (int i = args->start; i <= args->end; i++) {
        for (int j = 0; j < args->matrix->cols; j++) {
            rowSum += args->matrix->data[i][j];
        }
    }
    __sync_fetch_and_add(args->sum, rowSum);

    pthread_exit(NULL);
}

//sort a column in ascending order
void* sortColumnAscending(void* arguments) {
    ThreadArgs* args = (ThreadArgs*) arguments;

    int column[MAX_SIZE];
    for (int i = 0; i < args->matrix->rows; i++) {
        column[i] = args->matrix->data[i][args->start];
    }

    quickSort(column, 0, args->matrix->rows - 1);

    for (int i = 0; i < args->matrix->rows; i++) {
        args->matrix->data[i][args->start] = column[i];
    }

    int colSum = 0;
    for (int i = 0; i < args->matrix->rows; i++) {
        colSum += args->matrix->data[i][args->start];
    }
    __sync_fetch_and_add(args->sum, colSum);

    pthread_exit(NULL);
}

//sort a column in descending order
void* sortColumnDescending(void* arguments) {
    ThreadArgs* args = (ThreadArgs*) arguments;

    int column[MAX_SIZE];
    for (int i = 0; i < args->matrix->rows; i++) {
        column[i] = args->matrix->data[i][args->start];

    quickSort(column, 0, args->matrix->rows - 1);

    for (int i = 0; i < args->matrix->rows / 2; i++) {
        swap(&column[i], &column[args->matrix->rows - 1 - i]);
    }

    for (int i = 0; i < args->matrix->rows; i++) {
        args->matrix->data[i][args->start] = column[i];
    }
    int colSum = 0;
    for (int i = 0; i < args->matrix->rows; i++) {
        colSum += args->matrix->data[i][args->start];
    }
    __sync_fetch_and_add(args->sum, colSum);

    pthread_exit(NULL);
}
}

int main() {
    Matrix matrix;
    matrix.rows = 5;
    matrix.cols = 5;
    matrix.data[0][0] = 9;
    matrix.data[0][1] = 8;
    matrix.data[0][2] = 7;
    matrix.data[0][3] = 6;
    matrix.data[0][4] = 5;
    matrix.data[1][0] = 4;
    matrix.data[1][1] = 3;
    matrix.data[1][2] = 2;
    matrix.data[1][3] = 1;
    matrix.data[1][4] = 0;
    matrix.data[2][0] = 5;
    matrix.data[2][1] = 6;
    matrix.data[2][2] = 7;
    matrix.data[2][3] = 8;
    matrix.data[2][4] = 9;
    matrix.data[3][0] = 0;
    matrix.data[3][1] = 1;
    matrix.data[3][2] = 2;
    matrix.data[3][3] = 3;
    matrix.data[3][4] = 4;
    matrix.data[4][0] = 9;
    matrix.data[4][1] = 8;
    matrix.data[4][2] = 7;
    matrix.data[4][3] = 6;
    matrix.data[4][4] = 5;

    // Initialize the matrix addition
    int matrixAddition = 0;

    // Create the thread arguments
    ThreadArgs args1, args2, args3, args4;
    args1.matrix = &matrix;
    args1.start = 0;
    args1.end = 1;
    args1.sum = &matrixAddition;

    args2.matrix = &matrix;
    args2.start = 2;
    args2.end = 3;
    args2.sum = &matrixAddition;

    args3.matrix = &matrix;
    args3.start = 0;
    args3.end = 1;
    args3.sum = &matrixAddition;

    args4.matrix = &matrix;
    args4.start = 2;
    args4.end = 3;
    args4.sum = &matrixAddition;

    // Create the sorting threads
    pthread_t thread1, thread2, thread3, thread4;
    pthread_create(&thread1, NULL, sortRowAscending, (void*) &args1);
    pthread_create(&thread2, NULL, sortRowDescending, (void*) &args2);
    pthread_create(&thread3, NULL, sortColumnAscending, (void*) &args3);
    pthread_create(&thread4, NULL, sortColumnDescending, (void*) &args4);

    // Wait for the sorting threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    // Print the sorted matrix
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            printf("%d ", matrix.data[i][j]);
        }
        printf("\n");
    }

    // Print the matrix addition
    printf("Matrix Addition = %d\n", matrixAddition);

    return 0;
}
