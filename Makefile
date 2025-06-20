test_arraylist:
	clear
	gcc -Iheaders -o testarraylist src/test_arrayList.c  -lm
	./testarraylist
test_linear_algebra:
	clear
	gcc -Iheaders -o testlinalg src/test_linearAlgebra.c  -lm
	./testlinalg
test_activations:
	clear
	gcc -Iheaders -o testact src/test_activation.c -lm
	./testact
test_module:
	clear
	gcc -Iheaders -o tesmodule src/test_module.c -lm
	./tesmodule
