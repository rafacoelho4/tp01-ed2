#ifndef BTREE
#define BTREE
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../tipos.hpp"

#define M 2

typedef struct NodeT NodeT;
typedef struct NodeT Pointer;

struct NodeT{
    int n;
    Register r[(M*M)];
    Pointer* p[(M*M)+1];
};

void print(Pointer* tree);
void insert(Register reg, Pointer** tree,long long *comp);
bool search(Register* reg, Pointer* tree);
void insert_on_node(Pointer** tree, Register* reg, Pointer** right, long long* comp);
void find_to_insert(Register reg, Pointer** tree, bool *grew, Register* rr, Pointer** pr,long long *comp);
#endif
