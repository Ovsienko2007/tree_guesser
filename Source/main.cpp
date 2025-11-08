#include "tree.h"

int main(){
    tree_t tree = init_tree();

    DUMP(tree);

   
    check_elem(tree);
    check_elem(tree);

    DUMP(tree);

    destroy_tree(tree);
}