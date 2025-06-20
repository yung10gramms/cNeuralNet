#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#include"matrix.h"
#include<assert.h>
Matrix* linear(Matrix* x) {
    assert(x->rows==1||x->cols==1);
    Matrix* y = zeros_like(x);

    for (int i=0;i<x->cols*x->rows;i++) y->data[i]=x->data[i];
    return y;
} 

Matrix* linearJacobian(Matrix* x) {
    assert(x->rows==1||x->cols==1);
    return identity(x->cols*x->rows);
}

Matrix* ReLU(Matrix*x) {
    assert(x->rows==1||x->cols==1);
    Matrix* y = zeros_like(x);

    for (int i=0;i<x->cols*x->rows;i++) {
        if(x->data[i]>0)
            y->data[i]=x->data[i];
    }
    return y;
}

Matrix* reluJacobian(Matrix*x) {
    assert(x->rows==1||x->cols==1);
    Matrix* output = identity(x->cols*x->rows);
    for(int i=0;i<x->cols*x->rows;i++) {
        if(x->data[i]<=0) {
            setMatrix(output, i, i, 0);
        } 
    }
    return output;
}

#endif