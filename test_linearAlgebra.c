

#include "linearAlgebra.h"
#include<stdio.h>

void main() {
    int n=3;
    Matrix* I = identity(3);
    printMatrix(I);  
    free(I);

    Matrix* a = createMatrix(1, 2);
    setMatrix(a, 0, 0, 1);
    setMatrix(a, 0, 1, 2);
    printMatrix(a);

    Matrix* b = createMatrix(2, 1);
    setMatrix(b, 0, 0, 1);
    setMatrix(b, 1, 0, 2);
    printMatrix(b);

    Matrix* ab = matrixMul2(a, b);
    Matrix* ba = matrixMul2(b, a);
    
    printMatrix(ab);
    printMatrix(ba);
    
    free(a);free(b);
    free(ab);free(ba);

    /******************* norm ******************/
    Matrix* v = zeros(2, 1);
    v->data[0]=1; v->data[1]=1;
    printMatrix(v);
    printf("Norm (Eucl) of [1,1] is %f", norm(v, 2));
    free(v);
}