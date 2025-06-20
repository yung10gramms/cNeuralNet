# cNeuralNet
A library for constructing neural networks, written in pure C ⚙️

## Get started 🧭
One can run the unit tests using the makefile. The most informative is 
```bash
make test_module
```
### The typical workflow
In general, the structure that wraps the neural network is `module` in `module.h`. The least necassary information it needs to run is
#### 1️⃣ An activation function 
```C
act_fun linear_activation;
/* you can use the linear activation functions */
linear_activation.activate=linear;
linear_activation.jacobian=linearJacobian;
 
/* or the ReLU activation functions */
linear_activation.activate=ReLU;
linear_activation.jacobian=reluJacobian;
```
or even declare your own one. The convention made in this work is that all vectors and matrices are treated as matrices, similarly to matlab, and all activation functions are essentially vector fields, meaning functions $\mathbb{R}^N\rightarrow \mathbb{R}^N$, meaning their gradients are $\mathbb{R}^N\rightarrow \mathbb{R}^{N\times N}$. This unified approach allows for non element-wise activations such as `sigmoid`, etc.

#### 2️⃣ A loss function
As of now, the only loss function implemented is the $l_2$ squared one, being equal to $\mathcal{L}(y;\hat{y})=\Vert y-\hat{y}\Vert^2$ with a gradient of $\mathcal{L}(y;\hat{y})=2 ( y-\hat{y})$.
```c
cost_func loss_f;
/* use the implemented l2 squared loss function */
loss_f.cost=l2lossSquared;
loss_f.gradient=l2lossSquaredGrad;
```

#### 3️⃣ Apply them to the module 
Just make sure to add more than one layer so that backpropagation and all other fun things can occur.
```c
/* statically declare a module */
module neuralNet;
/* this function initializes all of its lists and other variables */
initializeModule(&neuralNet);
/* set a loss function */
neuralNet.loss_f=&loss_f;
append_layer(&neuralNet, 3, &linear_activation);
```
#### 4️⃣ Initialize an optimizer 📉 
The optimizer is responsible to look at the gradients and make a step towards the steepest descent to optimize the weights and biases. So far, only a SGD with a dynamic step rule has been integrated.
```c
Optimizer* opt = createOptimizer();
```
#### 5️⃣ Pass the data and let the training begin
```c
for(int i = 0; i < 100; i ++) {
    /* create data along with a label */
    x = gaussianMatrix(3, 1);
    y = linear_activation.activate(x);

    /* flush gradients */
    zero_grad(&neuralNet);
    /* insert values to the nn */
    insert(&neuralNet, x, y); 
    /* forward the values */ 
    forward(&neuralNet);  
    calc_loss(&neuralNet);
    printf("\nloss: %f\n", neuralNet.curr_loss);

    /* backpropagate */
    backward(&neuralNet);
    /* take a gradient step */
    dynamic_sgd(&neuralNet, opt);

    /* this is C so we must 
    respect the rules of C */
    free(x);
    free(y);
}
```