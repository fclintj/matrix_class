#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUMTHREADS 4

struct mat {
    int rows;
    int cols;
    int *mat;
};

void make_mat(int rows, int cols, struct mat* arr);
void multiply(struct mat* arr, struct mat* arr2, struct mat* result );
void fill_mat(struct mat* arr);
void fill_H_1(struct mat* arr);
int sum_array(int* arr, int size);
void print_mat(struct mat* arr);

int main() {
    struct mat arr, H_1, arr_result;
    make_mat(3,3,&arr);
    make_mat(5,5,&H_1);
    make_mat(1,1,&arr_result);
    fill_mat(&arr); 
    fill_H_1(&H_1); 
    
    
    print_mat(&H_1);
    
    free(arr.mat);
    free(H_1.mat);
    free(arr_result.mat);

    return 0;
}

void make_mat(int rows, int cols, struct mat* arr){
    arr->mat = (int *)malloc(rows * cols * sizeof(int));
    arr->rows = rows;
    arr->cols = cols;
}


void multiply(struct mat* arr, struct mat* arr2, struct mat* result ){
    int i,j,k;

    for (i=0;i<arr->rows;i++){
        for (j=0;j<arr2->cols;j++){
            for (k=0;k<arr->cols;k++){
                result->mat[i*arr->cols + j] += 
                    (arr->mat[i*arr->cols + k] * arr->mat[k*arr->cols + j]);
            }
        }
    }
}

void print_mat(struct mat* arr){
    // to change the width, change in printf statement, and make int 3 lines down +1
    int i,j;
    printf("┌");
    for (i = 0; i < 6*arr->cols; i++) {
        printf(" ");
    }
    printf("┐\n");

    for (i = 0; i < arr->rows; i++){
        printf("│");
        for (j = 0; j < arr->cols; j++){ 
            printf("%5.d ", arr->mat[i*arr->cols + j]);
        } 
            printf("│\n");
    }
    printf("└");
    for (int i = 0; i < 6*arr->cols; i++) {
        printf(" ");
    }
    printf("┘\n");
}

void fill_mat(struct mat* arr){
    int i, j, count = 0;
    for (i = 0; i < arr->rows; i++)
        for (j = 0; j < arr->cols; j++) 
         arr->mat[i*arr->cols + j] = ++count;
}

int sum_array(int* arr, int size){
    int sum = 0;
    for (int i = 0; i <size; i++) {
       sum += arr[i]; 
    }
    return sum;
}

void fill_H_1(struct mat* arr)
{
    double data[25] = 
    {  1,2,3,2,1,
       2,4,6,4,2,
       3,6,9,6,3,
       2,4,6,4,2,
       1,2,3,2,1  };
    
    for (int i = 0; i < arr->rows; i++) {
        for (int j = 0; j <arr->cols; j++) {
           arr->mat[i*arr->cols + j] = data[i+j];
        }
    }
}
