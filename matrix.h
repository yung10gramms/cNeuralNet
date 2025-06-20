#ifndef MATRIX_H
#define MATRIX_H

#include<stdlib.h>
#include<assert.h>


typedef struct matrix {
    double* data;
    int rows, cols;
}Matrix;

double getMatrix(Matrix* mat, int row, int col);
Matrix* createMatrix(int rows, int columns);
void incrMatrix(Matrix* mat, int row, int col, double value);
Matrix* zeros(int rows, int columns);
Matrix* identity(int n);
void setMatrix(Matrix* mat, int row, int col, double value);
int xy_to_index(int x, int y, int width);
void index_to_x_y(int index, int width, int* xy);
Matrix* zeros_like(Matrix*m);

Matrix* createMatrix(int rows, int columns) {
    assert(rows>=0 && columns>=0);
    Matrix* mat = (Matrix*)malloc(sizeof(Matrix));
    mat->data = (double*)malloc(rows*columns*sizeof(double));
    mat->rows = rows;
    mat->cols = columns;
    for (int i=0; i<rows; i++) for(int j=0; j<columns; j++) setMatrix(mat,i,j,0);
    return mat;
}

Matrix* zeros(int rows, int columns) {
    return createMatrix(rows, columns);
}

Matrix* identity(int n) {
    Matrix* mat = createMatrix(n,n);
    for(int i=0;i<n;i++) for(int j=0;j<n;j++) if(i==j) setMatrix(mat,i,j,1); else setMatrix(mat,i,j,0);
    return mat; 
}


void setMatrix(Matrix* mat, int row, int col, double value) {
    assert(row>=0 && row<mat->rows);
    assert(col>=0 && col<mat->cols);
    assert(mat->data);
    mat->data[xy_to_index(col, row, mat->cols)]=value;
}

void incrMatrix(Matrix* mat, int row, int col, double value) {
    assert(row>=0 && row<mat->rows);
    assert(col>=0 && col<mat->cols);
    assert(mat->data);
    mat->data[xy_to_index(col, row, mat->cols)]+=value;
}

double getMatrix(Matrix* mat, int row, int col) {
    assert(row>=0 && row<mat->rows);
    assert(col>=0 && col<mat->cols);
    assert(mat->data);
    return mat->data[xy_to_index(col, row, mat->cols)];
}

int xy_to_index(int x, int y, int width) {
    return x+y*width;
}

void index_to_x_y(int index, int width, int* xy) {
    xy[0] = (int) index/width;//y
    xy[1] = (int) index%width;
}

Matrix* zeros_like(Matrix*m) {
    return zeros(m->rows, m->cols);
}

#endif