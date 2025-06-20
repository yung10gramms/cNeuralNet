#include"matrix.h"
#include"module.h"
#include"activations.h"
#include"loss.h"
#include"optimizer.h"

#include<stdio.h>
#include<assert.h>
#include <time.h>

void main() {
    srand((unsigned int)time(NULL));
    /* Set an activation function */
    act_fun linear_activation;
    // linear_activation.activate=linear;
    // linear_activation.jacobian=linearJacobian;

    linear_activation.activate=ReLU;
    linear_activation.jacobian=reluJacobian;

    cost_func loss_f;
    loss_f.cost=l2lossSquared;
    loss_f.gradient=l2lossSquaredGrad;

    Matrix* x = zeros(3, 1);
    x->data[0]=1;
    printf("Initial matrix x:\n");
    printMatrix(x);

    Matrix* y = linear_activation.activate(x);
    printf("\nMatrix y=Linear(x):\n");
    printMatrix(y);
    assert(y->data[0]==1 && y->data[1]==0 && y->data[2]==0);

    Matrix* jacob = linear_activation.jacobian(x);
    printf("\nMatrix z=grad[Linear](x):\n");
    printMatrix(jacob);

    module neuralNet;
    initializeModule(&neuralNet);
    neuralNet.loss_f=&loss_f;

    append_layer(&neuralNet, 3, &linear_activation);

    // 2nd layer
    act_fun activation3;
    activation3.activate=linear;
    activation3.jacobian=linearJacobian;
    append_layer(&neuralNet, 100, &activation3);

    // 3rd layer
    act_fun activation2;
    activation2.activate=linear;
    activation2.jacobian=linearJacobian;

    append_layer(&neuralNet, 3, &activation2);

    Optimizer* opt = createOptimizer();
    ArrayList* losses = createWithSize(100);
    for(int i = 0; i < 100; i ++) {
        x = gaussianMatrix(3, 1);
        y = linear_activation.activate(x);

        zero_grad(&neuralNet);
        insert(&neuralNet, x, y);  
        forward(&neuralNet);  
        calc_loss(&neuralNet);
        printf("\nloss: %f\n", neuralNet.curr_loss);

        backward(&neuralNet);
        dynamic_sgd(&neuralNet, opt);

        aNode* n = createNode(i);
        double* loss_curr = (double*)malloc(sizeof(double));
        *loss_curr = neuralNet.curr_loss;
        n->data = &loss_curr;
        append(losses, n->data);

        free(x);
        free(y);

    }
    // printArrayList(losses);
    printf("\n\nloss=[");
    for(int i=0;i<100;i++){
        aNode n = getAtIndex(losses, i);
        double* loss__ = (double*)n.data; 
        printf("%f,", *loss__);
    }
    printf("]\n\n");
    freeList(losses);
    freeModule(&neuralNet);
}