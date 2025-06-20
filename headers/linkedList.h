#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdlib.h>
#include <stdbool.h> 
#include<stdio.h>
#define START_NODE_ID 1
#define END_NODE_ID -1


typedef struct linkedList
{
    struct linkedListNode* head, *tail; /* head and tail are dummy corner nodes */
    int size;
}lList;

typedef struct linkedListNode
{
    /* data */
    void* data;
    int id;
    struct linkedListNode*prev, *next;

}lNode;


void initList(lList* list) {
    
    // Placing 2 dummy nodes to avoid extra checkings
    list->head = (lNode*)malloc(sizeof(lNode));
    list->head->id = START_NODE_ID;

    list->tail = (lNode*)malloc(sizeof(lNode));
    list->tail->id = END_NODE_ID;

    list->head->prev = NULL;
    list->head->next = list->tail;
    list->tail->prev = list->head;
    list->tail->next = NULL;

    list->size = 0;
}

void addNode(lList* list, lNode* nn) {
    lNode *prev = list->tail->prev;
    list->tail->prev = nn;
    nn->next = list->tail;
    prev->next = nn;
    nn->prev = prev;
    list->size++;
}

void removeNode(lList* list, lNode* nn) {
    lNode *prev = nn->prev;
    lNode *next = nn->next;

    prev->next = next;
    next->prev = prev;

    nn->next = NULL;
    nn->prev = NULL;
    list->size--;
    // free(nn);
}

lNode* findById(lList* list, int id) {
    lNode *tmp = list->head->next;

    if (id==START_NODE_ID) return list->head;
    if (id==END_NODE_ID) return list->tail;
    
    while (tmp->id != END_NODE_ID) {
        if (tmp->id == id) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}

bool is_tail(lNode* nn) {
    return (nn->next->id==END_NODE_ID);
}

bool is_head(lNode* nn) {
    return (nn->prev->id==START_NODE_ID);
}

lNode* getTail(lList* list) {
    if (! list) return NULL;
    if(!list->tail) return NULL;
    if (list->tail->prev->id==START_NODE_ID)
        return NULL;
    return list->tail->prev;
}

lNode* getHead(lList* list) {
    if(! list) return NULL;
    if(!list->tail) return NULL;
    if (list->head->next->id==END_NODE_ID) return NULL;
    return list->head->next;
}

void* getIndex(lList* list, int index) {
    assert(list);
    assert(list->size>0);
    assert(index>=0 && index<list->size);
    int i = 0;
    lNode* iterator = list->head->next;
    for(i=0; i<list->size; i++) {
        if(i==index) {
            return iterator->data;
        }
        iterator=iterator->next;
    }
    /* we are not supposed to get here */
    return NULL;
}

void printList(lList* list) {
    lNode *tmp = list->head->next;

    if (tmp->id==END_NODE_ID) {printf("[]"); return;}

    printf("[");
    while (tmp->id != END_NODE_ID) {
        printf("%d", tmp->id);
        tmp = tmp->next;
        if (tmp->id != END_NODE_ID) {
            printf(", ");
        }
    }
    printf("]\n");
}

#endif 