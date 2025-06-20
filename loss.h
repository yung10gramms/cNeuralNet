#ifndef LOSS_H
#define LOSS_H
#include"linearAlgebra.h"
#include"matrix.h"
#include<assert.h>
#include<math.h>

/**
 * 
 * Returns (x-y)^T*(x-y)
*/
double l2lossSquared(Matrix* x, Matrix* yhat) {
    assert(x);
    assert(yhat);

    assert(x->cols==1 || x->rows==1);
    assert(yhat->cols==1||yhat->rows==1);
    assert(x->rows*x->cols==yhat->rows*yhat->cols);

    // double out = 0;
    // for (int i = 0; i<x->rows*x->cols; i++) {
    //     double tmp=(x->data[i]-yhat->data[i]);
    //     out+=tmp*tmp;
    // }

    return pow(norm(linearCombination(x, yhat, (double)1, (double)-1), 2),2);
    // return out;
}

/**
 * 
 * Returns 2*(x-y)
*/
Matrix* l2lossSquaredGrad(Matrix* x, Matrix* yhat) {
    assert(x);
    assert(yhat);

    assert(x->cols==1 || x->rows==1);
    assert(yhat->cols==1||yhat->rows==1);
    assert(x->rows*x->cols==yhat->rows*yhat->cols);

    return linearCombination(x, yhat, 2, -2);
}

#endif