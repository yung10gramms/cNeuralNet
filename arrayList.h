#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <assert.h>
#include<stdlib.h>
#include<stdio.h>
const double incr_factor = 0.5;
const int init_size = 10;

typedef struct arrayList
{
    int index;
    int bufferLen;
    struct arrayListNode *elems;
}ArrayList;

typedef struct arrayListNode {
    void* data;
    int id;
}aNode;

void initialize_arrayList(ArrayList* arr);

void initialize_arrayList(ArrayList* arr) {
    assert(arr);
    arr->index=0;
    arr->bufferLen=init_size;
    arr->elems=(aNode*)malloc(arr->bufferLen*sizeof(aNode));
}

/**
 * Allocates an array having the specified memory.
 */
ArrayList* createWithSize(int N) {
    ArrayList*arr=(ArrayList*)malloc(sizeof(ArrayList));
    arr->index=0;
    arr->bufferLen=N;
    arr->elems=(aNode*)malloc(arr->bufferLen*sizeof(aNode));
    return arr;
}

void append(ArrayList* arr, aNode* node) { 
    assert(arr); assert(node);
    if(arr->index >= arr->bufferLen) {
        // aNode* buffer = (aNode*)malloc(arr->bufferLen*sizeof(aNode));
        // for(int i=0; i<arr->index; i++) buffer[i]=arr->elems[i];
        aNode* buffer = arr->elems;
        arr->bufferLen = (int) ((1+incr_factor)*arr->bufferLen);
        aNode* temp = (aNode*)malloc(arr->bufferLen*sizeof(aNode));
        arr->elems = temp;
        for(int i=0; i<arr->index; i++) {arr->elems[i]=buffer[i];}
        free(buffer);
    }
    assert(arr->index < arr->bufferLen);
    
    arr->elems[arr->index] = *node; 
    arr->index++;
}

aNode getAtIndex(ArrayList* arr, int index) {
    assert(arr);
    assert(index<arr->index);
    return arr->elems[index];
}

void setAtIndex(ArrayList* arr, int index, aNode* node) {
    assert(arr); assert(index<arr->bufferLen);
    arr->elems[index] = *node;
}

void printArrayList(ArrayList* arr) {
    
    printf("[");
    for(int i=0;i<arr->index;i++) {
        printf("%d", getAtIndex(arr, i).id); if(i<arr->index-1) printf(", ");
    }
    printf("]\n");
}

aNode* createNode(int id) {
    aNode* node = (aNode*)malloc(sizeof(aNode));
    node->id=id;
    node->data=NULL;
    return node;
}

void freeList(ArrayList *arr) {
    // for(int i=0; i<arr->index; i++) {
    //     free(&getAtIndex(arr, i));
        
    // }
    free(arr->elems);
}

#endif