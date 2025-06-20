#ifndef RANDOM_H
#define RANDOM_H
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <math.h>
#include"matrix.h"


double randomUni() {
    return (double) rand()/RAND_MAX;
}

double exprnd() {
    double p = (double) rand()/RAND_MAX;
    return -log(1 - p)/log(exp(1));
}

double gaussianRandom() {
    double randomAngle = randomUni()*2*M_PI;
    double randomMagnitude = sqrt(2*exprnd());
    return cos(randomAngle)*randomMagnitude;
}

/**
 * 
 * Creates a matrix of i.i.d. standard Gaussian distributed entries.
*/
Matrix* gaussianMatrix(int rows, int cols) {
    Matrix* mat = (Matrix*)malloc(sizeof(Matrix));
    double* data = (double*)malloc(rows*cols*sizeof(double));
    mat->cols=cols;
    mat->rows=rows;
    mat->data=data;
    for(int i=0; i<rows; i++) {
        for (int j=0; j<cols; j ++)
            setMatrix(mat, i, j, gaussianRandom());
    }
    return mat;
}

#endif