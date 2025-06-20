#ifndef MODULE_H
#define MODULE_H

#include"linkedList.h"
#include"random.h"
#include"matrix.h"
#include"arrayList.h"
#include"linearAlgebra.h"
#include<assert.h>



typedef struct nn_layer
{
    Matrix* weights;
    Matrix* biases; 
    Matrix* state;
    
    Matrix* gradW, *gradb;

    int size;
    struct activation_function* activation;
}layer;

typedef struct nn_module
{
    lList *layers;
    Matrix *input, *yhat, *output;

    struct cost_function* loss_f;

    double curr_loss;
}module;

/**
 * Struct implementing an activation function. An activation function is not nessecarily
 * applied element-wisely, so we treat the as vector fields, that is functions R^n->R^n. 
 * Their Jacobian matrix contains the gradient of each output of the function.
 * For element-wise activations, like ReLU, the Jacobian is diagonal. For non element-wise 
 * activations, like softmax, this does not hold in general.
*/
typedef struct activation_function
{
    // Pointer to function taking double* and returning double*
    Matrix* (*activate)(Matrix *); 
    Matrix* (*jacobian)(Matrix *);
}act_fun;

typedef struct cost_function
{
    double (*cost)(Matrix*, Matrix*); 
    Matrix* (*gradient)(Matrix *, Matrix*);
}cost_func;

int numLayers(module* nn);

void initializeModule(module* nn) {
    nn->layers = (lList*)malloc(sizeof(lList));
    initList(nn->layers);

    nn->input=NULL;
    nn->output=NULL;
    nn->yhat=NULL;
    nn->loss_f=NULL;
}

void freeModule(module* nn) {
    // if(nn->input) free(nn->input);
    // if(nn->output) free(nn->output);
    // if(nn->yhat) free(nn->yhat);
    free(nn->layers);
}

void append_layer(module* nn, int n, act_fun* activation) 
{   
    printf("Checking if nn is null\n");
    assert(nn);

    printf("Checking if nn layers is null\n");
    assert(nn->layers);

    printf("Checking if activation is null\n");
    assert(activation);

    /* Create layer */
    layer* lay = (layer*)malloc(sizeof(layer)); // i am very curious to see if this will persist in memory
    lay->size = n;
    lay->activation = activation;
    
    // /* declare state an set it to zero */
    Matrix* state;
    state = zeros(n, 1); 
    lay->state = state;
    
    
    /* We need to wrap the layer 
    by a list node and append that
    to the rest of the list of layers */
    lNode *layerNode = (lNode *)malloc(sizeof(lNode));
    layerNode->data = lay;
    layerNode->id = nn->layers->size + 10;

    
    if (numLayers(nn)==0) {
        addNode(nn->layers, layerNode);
        printf("\nThis was the first layer!\n");
        return;
    } 

    
    lNode* tail = getTail(nn->layers);
    layer *last_layer = (layer*) tail->data;

    int w_dim1 = n, w_dim2 = last_layer->size;
    printf("%d", w_dim2); 
    addNode(nn->layers, layerNode);
    
    Matrix *weights;
    weights = gaussianMatrix(w_dim1, w_dim2);
    lay->weights = weights; 


    // TODO come back here 
    Matrix* gradW = zeros_like(weights);
    lay->gradW=gradW;

    // /* now it's time for the biases */
    Matrix *biases;
    biases=gaussianMatrix(n, 1);
    lay->biases = biases;

    Matrix* gradb = zeros_like(biases);
    lay->gradb=gradb;
    
}

int numLayers(module* nn) {
    assert(nn);
    assert(nn->layers);
    return nn->layers->size;
}

void initialize_weights() {
    //TODO put stuff in
}

/**
 * This function does not forward the input
 * 
*/
void insert(module* nn, Matrix* input, Matrix* label) {
    // TODO put some assertions here
    nn->input=input;
    nn->yhat=label;
}

/**
 * Method that forwards the already set input of the neural network.
 * 
*/
void forward(module* nn) {
    assert(nn->layers->size!=0);

    lNode *node=nn->layers->head;
    while(node->id!=END_NODE_ID) {
        if(node->id==START_NODE_ID){
            node=node->next;
            continue;
        }
        
        layer* curr_layer = (layer*) node->data;
        
        if(is_head(node)) { 
            curr_layer->state=nn->input;
            node=node->next;
            continue;
        }
        
        layer* prev_layer = (layer*) node->prev->data;
        Matrix* prev_state = (Matrix*) prev_layer->state; 

        Matrix *w = curr_layer->weights, *b=curr_layer->biases; 
        
        // printf("\nw.rows,w.cols=(%d,%d)\n", w->rows, w->cols);
        // printf("b.rows,b.cols=(%d,%d)\n\n", b->rows, b->cols);
        // printf("prev_state.rows,prev_state.cols=(%d,%d)\n\n", prev_state->rows, prev_state->cols);
        Matrix* w_x = matrixMul2(w, prev_state);
        Matrix* w_x_b = addMat(w_x, b); 
        curr_layer->state = curr_layer->activation->activate(w_x_b);
   
        free(w_x);
        free(w_x_b);
        // free(w_x_b_vec);

        node=node->next;
    }
    
    lNode* tail = getTail(nn->layers);
    assert(tail);
    layer* last_layer = (layer*)tail->data;
    nn->output=last_layer->state;

}

void calc_loss(module* nn) {
    nn->curr_loss=nn->loss_f->cost(nn->output, nn->yhat);
}

/** Flush gradients */
void zero_grad(module* nn) {
    lNode* iterator = getHead(nn->layers);
    assert(nn->layers->size>1);
    assert(iterator);
    iterator=iterator->next; // first layer does not have weights
    layer* lay = (layer*) iterator->data;
    while(iterator->id!=END_NODE_ID) {
        if(lay->gradW) {
            assert(lay->weights); free(lay->gradW); lay->gradW=zeros_like(lay->weights); 
        }
        if(lay->gradb) {
            assert(lay->biases); free(lay->gradb); lay->gradb=zeros_like(lay->biases);
        }
        iterator=iterator->next;
    }
}


void backward(module* nn) {
    // lList* gmap = ()
    assert(nn);
    int num_layers = numLayers(nn);
    if(num_layers <= 1) {
        printf("Module too small. Increase number of layers.\n"); return;
    }
    
    Matrix* g_map[num_layers];
    Matrix* z_map[num_layers-1];

    /* calculate loss if not already calculated */
    calc_loss(nn);

    Matrix* mat_last = nn->loss_f->gradient(nn->output, nn->yhat);
    
    g_map[num_layers-1] = mat_last;
    

    /* what is going on here? The module has 2 or more layers
    so there is a tail and there is a prev to the tail for sure */
    lNode* iterator = getTail(nn->layers);
    layer* right_layer = (layer*)iterator->data;
    iterator=iterator->prev;
    layer* cur_layer = (layer*)iterator->data;
    

    printf("\nNumber of layers %d\n", numLayers(nn));
    // TODO
    for(int i = num_layers-2; i >= 0; i --) {

        assert(cur_layer);
        z_map[i] = addMat(matrixMul2(right_layer->weights, cur_layer->state), right_layer->biases);

        
        Matrix* sigma_prime_z = cur_layer->activation->jacobian(z_map[i]);
        
        assert(g_map[i+1]->rows==sigma_prime_z->rows && g_map[i+1]->cols==1);
        /*
        since the gradient of the activation is a valid Jacobian
        the point-wise equivalent of Haadamard product is now dot
        product of the diagonal (in case of element-wise) matrix 
        grad[sigma] times g_map[i+1], which is essentially the propagated 
        gradient.  
        */
        Matrix* a = matrixMul2(sigma_prime_z, g_map[i+1]);
        /* free the dummy variable */
        free(sigma_prime_z);

        Matrix* stateT =  transpose(cur_layer->state);

        if(right_layer->gradW) free(right_layer->gradW);
        printf("a * x.T ");
        printShape(a);
        printShape(stateT);
        right_layer->gradW = matrixMul2(a, stateT);


        printf("\n[BACKPROP] norm of gradient %f\n", norm(right_layer->gradW,2));
        // printShape(right_layer->gradW);
        // printShape(right_layer->weights);
        Matrix* gradW_right = right_layer->gradW, *w_right = right_layer->weights;

        assert(gradW_right->cols==w_right->cols && gradW_right->rows==w_right->rows);

        
        if(stateT) free(stateT);
        
        // assert()
        
        if(right_layer->gradb) free(right_layer->gradb);
        
        right_layer->gradb = a;

        if(!cur_layer->weights) {
            printf("NO CURRENT LAYER weights");
        }
        
        assert(right_layer->weights && a);
        if(right_layer->weights && a){
            assert(right_layer->weights);
            assert(a);
            printf("[BACKPROP] we got here, index=%d\n", i);
            Matrix* w_T = transpose(right_layer->weights);
            g_map[i] = matrixMul2(w_T, a);
            free(w_T);
        }
        
        
        if(iterator->prev && iterator->prev->id!=START_NODE_ID) {
            
            right_layer = (layer*)iterator->data;
            assert(iterator->prev);
            
            assert(iterator->prev->data);
            iterator=iterator->prev;
            cur_layer = (layer*)iterator->data;
           
        } 
        // free(a);
    }

    // lNode* it = getTail(nn->layers);
    // printf("\n[BACKPROP]grads updated\n");
    // for(int j=0;j<nn->layers->size;j++) {
    //     layer* c = (layer*)it->data;
    //     if(!c->gradW) {
    //         printf("And layer %d has null grads.\n", (nn->layers->size-j));
    //         // return;
    //         continue;
    //     }
    //     it=it->prev;
    //     printf("Layer %d norm grad %f\n", (nn->layers->size-j), norm(c->gradW,2));
    // }

    // for (int i=0;i<nn->layers->size;i++){
    //     printMatrix(g_map[i]);
    //     // g_map
    // }

}

#endif
