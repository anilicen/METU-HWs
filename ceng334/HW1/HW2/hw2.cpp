#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include<unistd.h>   
#include "hw2_output.h"

struct Matrix
{
    int x;
    int y;
    int **matrix;
    Matrix(int x, int y) : x(x), y(y)
    {
        matrix = new int *[x];
        for (int i = 0; i < x; ++i)
        {
            matrix[i] = new int[y];
        }
    }
};

Matrix firstMatrix(0, 0);
Matrix secondMatrix(0, 0);
Matrix thirdMatrix(0, 0);
Matrix forthMatrix(0, 0);
Matrix firstResult(0, 0);
Matrix secondResult(0, 0);
Matrix finalMatrix(0, 0);
int x, y;
int* columnList;
sem_t* rowSemaphores;
sem_t** columnSemaphores;
sem_t* mutex;
void getInput()
{
    std::cin >> x >> y;
    Matrix fast1(x, y);
    Matrix fast2(x, y);

    firstMatrix = fast1;
    secondMatrix = fast2;

    for (int x = 0; x < firstMatrix.x; x++)
    {
        for (int y = 0; y < firstMatrix.y; y++)
        {
            std::cin >> firstMatrix.matrix[x][y];
        }
    }

    std::cin >> secondMatrix.x;
    std::cin >> secondMatrix.y;

    for (int x = 0; x < secondMatrix.x; x++)
    {
        for (int y = 0; y < secondMatrix.y; y++)
        {
            std::cin >> secondMatrix.matrix[x][y];
        }
    }

    std::cin >> x;
    std::cin >> y;

    Matrix fast3(x, y);
    Matrix fast4(x, y);

    thirdMatrix = fast3;
    forthMatrix = fast4;

    for (int x = 0; x < thirdMatrix.x; x++)
    {
        for (int y = 0; y < thirdMatrix.y; y++)
        {
            std::cin >> thirdMatrix.matrix[x][y];
        }
    }

    std::cin >> forthMatrix.x;
    std::cin >> forthMatrix.y;

    for (int x = 0; x < forthMatrix.x; x++)
    {
        for (int y = 0; y < forthMatrix.y; y++)
        {
            std::cin >> forthMatrix.matrix[x][y];
        }
    }
}

void printMatrix(Matrix matrix)
{
    for (int j = 0; j < matrix.x; j++)
    {
        for (int i = 0; i < matrix.y; i++)
        {
            std::cout << matrix.matrix[j][i] << " ";
        }
        std::cout << "\n";
    }
}

void *addMatrixFirst(void *arg)
{
    int row = (long)arg;
    int result;
    for (int i = 0; i < firstMatrix.y; i++)
    {   
    

        result = firstMatrix.matrix[row][i] + secondMatrix.matrix[row][i];
        firstResult.matrix[row][i] = result;
        hw2_write_output(0, row, i, result);
    }
        sem_post(&(rowSemaphores[row]));
    // wait(column signal)
    //signal row is done.
}

void *addMatrixSecond(void *arg)
{
    int row = (long)arg;
    int result;
       
    for (int i = 0; i < thirdMatrix.y; i++)
    {
    

        

        result = thirdMatrix.matrix[row][i] + forthMatrix.matrix[row][i];
        secondResult.matrix[row][i] = result;
        hw2_write_output(1, row, i, result);
        sem_wait(&(mutex[i]));
        columnList[i]++;
        if (columnList[i] == thirdMatrix.x) 
        {
            for(int jj=0; jj<firstMatrix.x;jj++){
         
            sem_post(&(columnSemaphores[i][jj])); 
            }
        }
        sem_post(&(mutex[i]));
    }
}

void *multiplicate(void *args)
{   

    int row = (long) args;
    
    int first_y=firstResult.y;
    int second_y = secondResult.y;
    int result;
    for(int j = 0 ; j<second_y;j++)
    {   
    result = 0;
    sem_wait(&(columnSemaphores[j][row]));        
    for(int i=0; i<first_y;i++){
        if(i==0 && j == 0)
            sem_wait((&rowSemaphores[row]));
        result += firstResult.matrix[row][i]*secondResult.matrix[i][j];
        }
        
        finalMatrix.matrix[row][j] = result; 
        hw2_write_output(2, row, j, result);

    }
}

void initSemaphores(int firstRows, int secondColumns){
    for(int i = 0; i<firstRows; i++)
        {
            sem_init(&(rowSemaphores[i]),0600,0);
        }
    for(int i = 0; i<secondColumns; i++){
    for(int jj = 0 ; jj< firstRows; jj++){

        sem_init(&(columnSemaphores[i][jj]),0600,0);
        }
    }
    for(int i = 0; i<secondColumns; i++){
        sem_init(&(mutex[i]),0600,1);   
    }

return;
}

void closeSemaphores(int firstRows, int secondColumns){
    for(int i = 0; i<firstRows; i++)
        {
            sem_close(&(rowSemaphores[i]));
        }
    for(int i = 0; i<secondColumns; i++){
    for(int jj = 0 ; jj< firstRows; jj++){

        sem_close(&(columnSemaphores[i][jj]));
        }
    }

    for(int i = 0; i<secondColumns; i++){
        sem_close(&(mutex[i]));   
    }

}

int main()
{   
    hw2_init_output();
    getInput();
    
    int firstRows = firstMatrix.x;
    int firstColumns = firstMatrix.y;
    int secondRows = thirdMatrix.x;
    int secondColumns = thirdMatrix.y;
    int multiplicationCount = firstRows;

    rowSemaphores = (sem_t*) malloc(firstRows * sizeof(sem_t));
    columnSemaphores = (sem_t**) malloc(secondColumns * sizeof(sem_t*));
    
    for(int i = 0 ; i<secondColumns;i++){
        columnSemaphores[i] = (sem_t*) malloc(firstRows * sizeof(sem_t));
    }

    mutex = (sem_t*) malloc(secondColumns * sizeof(sem_t));

    columnList = (int*) malloc(secondColumns * sizeof(int));

    for (int i = 0 ; i<secondRows;i++){
        columnList[i] = 0; 
    }   
    Matrix fR(firstRows, firstColumns);
    Matrix sR(secondRows, secondColumns);
    Matrix fM(firstRows, secondColumns);
    
    firstResult = fR;
    secondResult = sR;
    finalMatrix = fM;

    pthread_t additionFirst[firstRows];
    pthread_t additionSecond[secondRows];
    pthread_t multiplication[multiplicationCount];

    initSemaphores(firstRows, secondColumns);
    
    for (int i = 0; i < firstRows; i++)
    {
        int row = i;
        pthread_create(&(additionFirst[i]), NULL, addMatrixFirst, (void *)row); // row must be in list
    }

    for (int i = 0; i < secondRows; i++)
    {
        int row = i;
        pthread_create(&(additionSecond[i]), NULL, addMatrixSecond, (void *)row); // row must be in list
    }

    for (int i = 0; i < multiplicationCount; i++)
    {
        int row = i;
        pthread_create(&(multiplication[i]), NULL, multiplicate, (void *)row); // row must be in list
    }


    for (int i = 0; i < firstRows; i++)
    {   
        pthread_join(additionFirst[i], NULL);
    }

    for (int i = 0; i < secondRows; i++)
    {   
        pthread_join(additionSecond[i], NULL);
        
    }

    for (int i = 0; i < multiplicationCount; i++)
    {
        pthread_join(multiplication[i], NULL);

    }
    
    printMatrix(finalMatrix);
    closeSemaphores(firstRows, secondColumns);

    return 0;
}
