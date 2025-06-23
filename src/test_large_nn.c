#include"matrix.h"
#include"module.h"
#include"activations.h"
#include"loss.h"
#include"optimizer.h"

#include<stdio.h>
#include<assert.h>
#include <time.h>

/**
 * 
 * apply an affine transformation on the data
*/
Matrix* y_x(Matrix* x, Matrix* A, Matrix* b) {
    return addMat(matrixMul2(A, x), b);
}

/**
 * 
 * Export loss vector into a python script.
*/
void export_losses(ArrayList* losses) {
    FILE *file = fopen("output.py", "w");
    
    fprintf(file, "from matplotlib import pyplot as plt\n\n\n");
    
    fprintf(file, "\n\nloss=[");
    for(int i=0;i<losses->index;i++){
        aNode n = getAtIndex(losses, i);
        double* loss__ = (double*)n.data; 
        fprintf(file, "%f,", *loss__);
    }
    fprintf(file, "]\n\n");
    fprintf(file, "plt.semilogy(loss)\nplt.title('Loss at each iteration - semilogy scale')\nplt.ylabel('loss')\nplt.xlabel('iteration')");
    fclose(file);
}

void main() {
    srand((unsigned int)time(NULL));
   
    cost_func loss_f;
    loss_f.cost=l2lossSquared;
    loss_f.gradient=l2lossSquaredGrad;

    int layer_dimension = 100, input_dimension = 3, num_iterations=200;

    Matrix* x = zeros(input_dimension, 1);
    x->data[0]=1;
    printf("Initial matrix x:\n");
    printMatrix(x);

    Matrix* y;
    Matrix* A=gaussianMatrix(layer_dimension, input_dimension);
    Matrix* b=gaussianMatrix(layer_dimension,1);

    module neuralNet;
    initializeModule(&neuralNet);
    neuralNet.loss_f=&loss_f;


    int num_layers = 100;
    act_fun *activations_array[num_layers];
    for (int i=0; i<num_layers; i++) {
        activations_array[i] = getReLU();
        if (i==0) {
            append_layer(&neuralNet, input_dimension, activations_array[i]); continue;
        }
        append_layer(&neuralNet, layer_dimension, activations_array[i]);
    }

    Optimizer* opt = createOptimizer();
    ArrayList* losses = createWithSize(num_iterations);
    time_t time_start = time(NULL), time_end;
    for(int i = 0; i < num_iterations; i ++) {
        /* Generate random data and labels */
        x = gaussianMatrix(input_dimension, 1);
        y = y_x(x, A, b);
        /* Flush gradients to be safe */
        zero_grad(&neuralNet);
        /* Insert and forward input */
        insert(&neuralNet, x, y);  
        forward(&neuralNet);  
        /* Calculate loss */
        calc_loss(&neuralNet);
        /* Backpropagate */
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
    time_end=time(NULL);
    printf("\n\nloss=[");
    for(int i=0;i<losses->index;i++){
        aNode n = getAtIndex(losses, i);
        double* loss__ = (double*)n.data; 
        printf("%f,", *loss__);
    }
    printf("]\n\n");
    printf("time elapsed to train an ANN with\n\tlayers: %d\n\tDimension: %d\n\tNum iterations: %d\n\n%ld seconds\n\n", 
        num_layers, layer_dimension,num_iterations, time_end-time_start);
    export_losses(losses);

    freeList(losses);
    for (int i=0;i<num_layers;i++){free(activations_array[i]);}
    freeModule(&neuralNet);
}