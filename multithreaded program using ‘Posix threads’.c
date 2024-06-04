#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 100

typedef struct {
    int row;
    int col;
    int** matrix1;
    int** matrix2;
    int** result;
    pthread_mutex_t* mutex;
} ThreadData;




void* multiply(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int row = data->row;
    int col = data->col;
    int** matrix1 = data->matrix1;
    int** matrix2 = data->matrix2;
    int** result = data->result;
    pthread_mutex_t* mutex = data->mutex;

    int sum = 0;
    for (int i = 0; i < col; i++) {
        sum = 0;
        for (int j = 0; j < row; j++) {
            sum += matrix1[row][j] * matrix2[j][i];
        }
        pthread_mutex_lock(mutex);
        result[row][i] = sum;
        pthread_mutex_unlock(mutex);
    }

    pthread_exit(NULL);
}

int main() {
    int m1, n1, m2, n2;

    printf("Enter the number of rows for matrix 1: ");
    scanf("%d", &m1);
    printf("Enter the number of columns for matrix 1: ");
    scanf("%d", &n1);

    printf("Enter the number of rows for matrix 2: ");
    scanf("%d", &m2);
    printf("Enter the number of columns for matrix 2: ");
    scanf("%d", &n2);

    if (n1 != m2) {
        printf("Invalid matrix sizes for multiplication.\n");
        return 1;
    }

    int** matrix1 = (int**)malloc(m1 * sizeof(int*));
    int** matrix2 = (int**)malloc(m2 * sizeof(int*));
    int** result = (int**)malloc(m1 * sizeof(int*));

    for (int i = 0; i < m1; i++) {
        matrix1[i] = (int*)malloc(n1 * sizeof(int));
        result[i] = (int*)malloc(n2 * sizeof(int));

        for (int j = 0; j < n1; j++) {
            printf("Enter the value for matrix 1[%d][%d]: ", i, j);
            scanf("%d", &matrix1[i][j]);
        }
    }

    for (int i = 0; i < m2; i++) {
        matrix2[i] = (int*)malloc(n2 * sizeof(int));

        for (int j = 0; j < n2; j++) {
            printf("Enter the value for matrix 2[%d][%d]: ", i, j);
            scanf("%d", &matrix2[i][j]);
        }
    }

    pthread_t threads[MAX_THREADS];
    ThreadData threadData[MAX_THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    if (m1 > MAX_THREADS) {
        printf("Number of threads exceeds the maximum limit.\n");
        return 1;
    }

    for (int i = 0; i < m1; i++) {
        threadData[i].row = i;
        threadData[i].col = n2;
        threadData[i].matrix1 = matrix1;
        threadData[i].matrix2 = matrix2;
        threadData[i].result = result;
        threadData[i].mutex = &mutex;

        if (pthread_create(&threads[i], NULL, multiply, (void*)&threadData[i]) != 0) {
            printf("Failed to create thread %d\n", i);
            return 1;
        }
    }

    for (int i = 0; i < m1; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Failed to join thread %d\n", i);
            return 1;
        }
    }

    // Print the resulting matrix
    printf("Resulting matrix:\n");
    for (int i = 0; i < m1; i++) {
        for (int j = 0; j < n2; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    // Free allocated memory
    for (int i = 0; i < m1; i++) {
        free(matrix1[i]);
        free(result[i]);
    }
    for (int i = 0; i < m2; i++) {
        free(matrix2[i]);
    }
    free(matrix1);
    free(matrix2);
    free(result);

    return 0;
}
