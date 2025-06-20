#include <assert.h>
#include<stdlib.h>
#include<stdio.h>

#include"arrayList.h"
void fillList(ArrayList*arrList, int N);
void main() {
    ArrayList arrList;
    initialize_arrayList(&arrList);
    printArrayList(&arrList);
    int N=20;
    fillList(&arrList,N);
    printArrayList(&arrList);
    freeList(&arrList);

    printf("\nBuffer length: %d\n", arrList.bufferLen);
}


void fillList(ArrayList*arrList, int N) {
    for(int i=0;i<N;i++){
        aNode* n = createNode(i);
        append(arrList, n);
        // printf("\nBuffer length: %d\n", arrList->bufferLen);
    }
}