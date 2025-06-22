#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include"linkedList.h"
#include"module.h"
#include"matrix.h"
#include"linearAlgebra.h"
#include<stdlib.h>

#define DYNAMIC_STEP 1
#define CONSTANT_STEP 2

/**
 * The optimizer has the role of handling the weights and biases 
 * and applying a gradient step to them accordingly,
 * in order to approach the steepest descent.
 * 
*/

typedef struct optimizer
{
    int k, step_rule;
    double lr;
}Optimizer;

Optimizer* createOptimizer() {
    Optimizer* opt = (Optimizer*)malloc(sizeof(Optimizer));
    opt->k=1;
    opt->lr=0.9;

    opt->step_rule = DYNAMIC_STEP;
}

void dynamic_sgd(module* nn, Optimizer* opt){
    assert(nn);
    assert(nn->layers);
    lNode* iterator = getTail(nn->layers);
    assert(iterator);

    Matrix* gradW, *gradb, *weights_tmp, *biases_tmp;
    layer* cur_layer;
    opt->k ++;
    opt->lr=0.9;
    while(iterator->prev->id != START_NODE_ID) {
        
        cur_layer = (layer*) iterator->data;
        if(!cur_layer->gradW || !cur_layer->gradb || !cur_layer->biases || !cur_layer->weights) {
            assert(iterator->prev->id==START_NODE_ID);
            return;
        }

        /* this assertion seems reduntant */
        assert(iterator->prev->id!=START_NODE_ID);
        
        /***** weights ********/
        gradW = (Matrix*) cur_layer->gradW;
        // printf("\n[OPTIMIZER] NORM of gradient %f\n",norm(cur_layer->gradW,2));
        if(norm(gradW,2)>0 && opt->step_rule == DYNAMIC_STEP)
            opt->lr = (double) 1/(opt->k * norm(gradW, 2));
        // printf("[OPTIMIZER]lr=%f\n", opt->lr);
        weights_tmp = cur_layer->weights;
        cur_layer->weights = linearCombination(weights_tmp, gradW, (double) 1, (double) -opt->lr);
        cur_layer->weights = linearCombination(cur_layer->weights, zeros_like(cur_layer->weights), (double)1/norm(cur_layer->weights, 2), 0);

        if(weights_tmp) free(weights_tmp);
        
        /****** biases ******/
        gradb = (Matrix*) cur_layer->gradb;
        if(norm(gradb,2)>0 && opt->step_rule == DYNAMIC_STEP)
            opt->lr = (double) 1/(opt->k * norm(gradb, 2));
        biases_tmp=cur_layer->biases;
        cur_layer->biases = linearCombination(biases_tmp, gradb, (double) 1, (double) -opt->lr);
        cur_layer->biases = linearCombination(cur_layer->biases, zeros_like(cur_layer->biases), (double)1/norm(cur_layer->biases, 2), 0);

        /* go to previous layer */
        iterator=iterator->prev;

    }

}

#endif