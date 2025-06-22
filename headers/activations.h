#ifndef ACTIVATIONS_H
#define ACTIVATIONS_H

#include"matrix.h"
#include"module.h"
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

/**
 * 
 * dynamically allocate a new activation function.
*/
act_fun* activation() {
    act_fun* new_act = (act_fun*)malloc(sizeof(act_fun));
    return new_act;
}

/**
 * 
 * dynamically allocate a linear activation function.
*/
act_fun* getLinearActivation() {
    act_fun* new_act=activation();
    new_act->activate=linear;
    new_act->jacobian=linearJacobian;
    return new_act;
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

/**
 * 
 * dynamically allocate a ReLU activation function.
*/
act_fun* getReLU() {
    act_fun* new_act = activation();
    new_act->activate=ReLU;
    new_act->jacobian=reluJacobian;
    return new_act;
}

#endif