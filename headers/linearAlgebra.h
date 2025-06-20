#ifndef LINEARALGEBRA_H
#define LINEARALGEBRA_H
#include"matrix.h"
#include<stdio.h>
#include <math.h>

Matrix* hadamard(Matrix* a, Matrix* b) {
    assert(a);
    assert(b);
    assert(a->cols==b->cols && a->rows==b->rows);

    Matrix* out = zeros_like(a);
    for(int i=0; i<a->cols*a->rows; i++) {
        out->data[i]=a->data[i]*b->data[i];
    }
    return out;
}

Matrix* matrixMul2(Matrix* a, Matrix* b) {
    assert(a);
    assert(b);
    Matrix* y = createMatrix(a->rows, b->cols);

    assert(b->rows==a->cols);
    // assert(a->rows==b->cols);
    int n = a->rows, m=a->cols, k=b->cols;
    for (int i = 0; i < n; i ++) {
        for(int j = 0; j < k; j ++) {
            setMatrix(y, i, j, 0);
            for(int p = 0; p < m; p ++) {
                double a_ip = getMatrix(a, i, p), b_pi = getMatrix(b, p, j);
                double ab = a_ip*b_pi;
                incrMatrix(y, i, j, ab);
            }
        }
    }
    return y;
}

Matrix* transpose(Matrix* m) {
    assert(m);
    Matrix* out = createMatrix(m->cols, m->rows);
    
    for(int i=0; i<m->rows; i ++) {
        for(int j=0; j<m->cols; j++) {
            setMatrix(out, j, i, getMatrix(m, i, j));
        }
    }
    return out;
}

Matrix* addMat(Matrix *a, Matrix *b)
{
    Matrix* out = createMatrix(a->rows, a->cols);
    for(int i = 0; i < a->rows; i ++) 
        for(int j = 0; j < a->cols; j ++)
            setMatrix(out, i, j, getMatrix(a, i, j) + getMatrix(b, i, j)); 
    
    return out;
} 

/**
 * Returns c1*a + c2*b
 * 
*/
Matrix* linearCombination(Matrix *a, Matrix *b, double c1, double c2)
{
    Matrix* out = createMatrix(a->rows, a->cols);
    for(int i = 0; i < a->rows; i ++) 
        for(int j = 0; j < a->cols; j ++)
            setMatrix(out, i, j, c1*getMatrix(a, i, j) + c2*getMatrix(b, i, j)); 
    
    return out;
} 

void printMatrix(Matrix* x) {
    printf("[");
    
    int m = x->rows;
    int n = x->cols;
    for (int j = 0; j < m; j ++) {
        printf("[");
        for(int i = 0; i < n; i ++) {
            printf("%f", getMatrix(x,j,i));
            if(i != n - 1) {
                printf(", ");
            } else if (i==n-1) printf("]");
        }  
         if (j!=m-1)  printf("\n");
    }printf("]\n");
    
}

void printShape(Matrix* x) {
    printf("(%d, %d)", x->rows, x->cols);
}

/**
 * 
 * Calculate the p-norm of a matrix.
*/
double norm(Matrix* vec, double p) {
    double out = 0;
    assert(vec);
    assert(vec->data);
    // assert(vec->cols==1 || vec->rows==1);
    for(int i=0; i<vec->cols*vec->rows;i++) {
        out+=pow(vec->data[i],p);
    }
    return pow((double) out, (double) 1/p);
}

#endif