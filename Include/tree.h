#ifndef TREE_H_
#define TREE_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define DUMP(tree) show_dump(tree, dump_position {__FILE__, __FUNCTION__,  __LINE__})

typedef char * tree_elem_t;
static const int kMaxAnsLen         = 64;
static const int kStartPrevElemSize = 16;

struct node_t {
    tree_elem_t  value;
    node_t      *ans1;
    node_t      *ans2;
};

enum tree_error_t {
    eNoError       = 0,
    eIncoorectNode = 1,
    eCicleInTree   = 2,
    eNullPtr       = 3,
};

enum check_cicles_mod {
    eStandart      = 0,
    eStartNewCheck = 1,
};

typedef node_t* tree_t;

struct dump_position {
    const char *file;
    const char *func;
    int line;
};

struct prev_elem {
    size_t   capacity;
    size_t   size;
    node_t **data;
};

tree_error_t destroy_tree(tree_t tree);
tree_error_t check_elem(tree_t tree);
tree_error_t add_elem(tree_t tree);

node_t *init_node();
tree_t  init_tree();
void show_dump(tree_t tree, dump_position position);

void print_data(tree_t tree, const char *file_name);


tree_error_t tree_verify(tree_t tree);
#endif