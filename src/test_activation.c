#include"matrix.h"
#include"module.h"
#include"activations.h"
#include<stdio.h>
#include<assert.h>

void main() {
    act_fun linear_activation;
    linear_activation.activate=linear;
    linear_activation.jacobian=linearJacobian;

    Matrix* x = zeros(3, 1);
    x->data[0]=1;
    printf("Initial matrix x:\n");
    printMatrix(x);

    Matrix*y = linear_activation.activate(x);
    printf("\nMatrix y=Linear(x):\n");
    printMatrix(y);
    assert(y->data[0]==1 && y->data[1]==0 && y->data[2]==0);

    Matrix* jacob = linear_activation.jacobian(x);
    printf("\nMatrix z=grad[Linear](x):\n");
    printMatrix(jacob);

    free(x);
    free(y);
    free(jacob);
}