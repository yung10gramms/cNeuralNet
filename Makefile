test_arraylist:
	clear
	gcc -o testarraylist test_arrayList.c  -lm
	./testarraylist
test_linear_algebra:
	clear
	gcc -o testlinalg test_linearAlgebra.c  -lm
	./testlinalg
test_activations:
	clear
	gcc -o testact test_activation.c -lm
	./testact
test_module:
	clear
	gcc -o tesmodule test_module.c -lm
	./tesmodule
