#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DUMP(tree) show_dump(tree, dump_position {__FILE__, __FUNCTION__,  __LINE__})

typedef char * tree_elem_t;
static const int kMaxAnsLen = 64;

struct node_t {
    tree_elem_t value;
    node_t *ans1;
    node_t *ans2;
};

typedef node_t* tree_t;

struct dump_position {
    const char *file;
    const char *func;
    int line;
};

void destroy_tree(tree_t *tree);
void check_elem(tree_t tree);
void add_elem(tree_t tree);

node_t *init_node();
tree_t  init_tree();
void show_dump(tree_t tree, dump_position position);

void print_data(tree_t tree, const char *file_name);

#endif