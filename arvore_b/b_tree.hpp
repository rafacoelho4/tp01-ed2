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
void b_tree(int key, const char* name);
void print(Pointer* tree, int key);
void insert(Register reg, Pointer** tree,long long *comp);
bool search(Register* reg, Pointer* tree,long long *comp);
void insert_on_node(Pointer** tree, Register* reg, Pointer** right, long long* comp);
void find_to_insert(Register reg, Pointer** tree, bool *grew, Register* rr, Pointer** pr,long long *comp);
#endif
