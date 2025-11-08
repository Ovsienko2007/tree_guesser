#include "tree.h"

static bool get_answer();
static void clean_buffer();

static tree_error_t check_nodes(tree_t tree);
static tree_error_t check_cicles(tree_t tree, int rec_deep = 0);
static bool check_prev(node_t *el, check_cicles_mod mod);

tree_error_t destroy_tree(tree_t tree){
    assert(tree);
    if (!tree) return eNullPtr;

    if (tree->ans1) {
        destroy_tree(tree->ans1);
    }
    if (tree->ans2) {
        destroy_tree(tree->ans2);
    }

    free(tree->value);
    free(tree);

    return eNoError;
}

void print_data(tree_t tree, const char *file_name) {
    assert(tree);
    if (!tree) return;

    static int rec_deep = 0;
    static FILE *data_stream = nullptr;
    if (rec_deep == 0){
        data_stream = fopen(file_name, "w");
    }
    rec_deep++;

    fprintf(data_stream, "(\"%s\" ", tree->value);

    if (tree->ans1) {
        print_data(tree->ans1, file_name);
    }
    else{
        fprintf(data_stream, "Nil ");
    }

    if (tree->ans2) {
        print_data(tree->ans2, file_name);
    }
    else{
        fprintf(data_stream, "Nil ");
    }

    fprintf(data_stream, ")");

    rec_deep--;
}

tree_error_t check_elem(tree_t sub_tree){
    if (tree_verify(sub_tree)){
        DUMP(sub_tree);
        return tree_verify(sub_tree);
    }

    if (sub_tree->ans1 == nullptr && sub_tree->ans2 == nullptr){
        printf("Ответ: %s? (Да/Нет)\n", sub_tree->value);

        if (!get_answer()){
            printf("Хотите добавить новый элемент? (Да/Нет)\n");

            if (get_answer()) add_elem(sub_tree);
        }
    }
    else{
        printf("То что вы загадали %s? (Да/Нет)\n", sub_tree->value);

        if (get_answer()){
            return check_elem(sub_tree->ans1);
        }
        else{
            return check_elem(sub_tree->ans2);
        }
    }

    if (tree_verify(sub_tree)){
        DUMP(sub_tree);
        return tree_verify(sub_tree);
    }
    return eNoError;
}

tree_t init_tree(){
    char *value = (char *)calloc(kMaxAnsLen, sizeof(char));
    strcpy(value, "Неизветсно что");

    tree_t tree = init_node();
    tree->value = value;

    return tree;
}

tree_error_t add_elem(node_t *node){
    assert(node);
    char *user_answer1 = (char *)calloc(kMaxAnsLen, sizeof(char));
    char *user_answer2 = (char *)calloc(kMaxAnsLen, sizeof(char));

    node_t *ans1 = init_node();
    node_t *ans2 = init_node();

    printf("Напишите то, что вы загадали: ");
    clean_buffer();
    fgets(user_answer1, kMaxAnsLen, stdin);
    *strchr(user_answer1, '\n') = '\0'; 

    ans1->value = user_answer1;
    ans2->value = node->value;

    node->ans1  = ans1;
    node->ans2  = ans2;

    printf("Продолжите фразу о том, чем то, что вы загадали, отличается от предложенного вараината: \nОн ");
    fgets(user_answer2, kMaxAnsLen, stdin);
    *strchr(user_answer2, '\n') = '\0';
    node->value = user_answer2;

    if (tree_verify(node)){
        DUMP(node);
        return tree_verify(node);
    }
    return eNoError;
}

node_t *init_node(){
    node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
    return new_node;
}

static bool get_answer(){
    char answer[kMaxAnsLen] = {};

    while (true){
        scanf("%s", answer);
        if (strcmp(answer, "Да") == 0){
            return true;
        }
        else if (strcmp(answer, "Нет") == 0){
            return false;
        }
        else{
            printf("Incorrect answer\n");
        }
    }
}

static void clean_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

tree_error_t tree_verify(tree_t tree){
    if (!tree)              return eNullPtr;

    if (check_nodes(tree))  return eIncoorectNode;

    if (check_cicles(tree)) return eCicleInTree;

    return eNoError;
}

static tree_error_t check_nodes(tree_t tree){
    if ((tree->ans1 != 0) ^ (tree->ans1 != 0)){
        return eIncoorectNode;
    }
    if (tree->ans1){
        tree_verify(tree->ans1);
    }
    if (tree->ans2){
        tree_verify(tree->ans2);
    }
    
    return eNoError;      
}

static tree_error_t check_cicles(tree_t tree, int rec_deep){
    tree_error_t error = eNoError;
    rec_deep++;

    if (rec_deep == 1){
        if (check_prev(tree, eStartNewCheck)) return eCicleInTree;
    }
    else{
        if (check_prev(tree, eStandart)) return eCicleInTree;
    }

    if (!error && tree->ans1){
        error = check_cicles(tree->ans1, rec_deep);
    }
    if (!error && tree->ans2){
        error = check_cicles(tree->ans2, rec_deep);
    }

    rec_deep--;
    return error;      
}

static bool check_prev(node_t *el, check_cicles_mod mod){
    static prev_elem prev = {};

    if (mod == eStartNewCheck){
        if (prev.data) free(prev.data);
        
        prev.data = (node_t **)calloc(kStartPrevElemSize, sizeof(node_t *));
        
        prev.capacity = kStartPrevElemSize;
        prev.size = 0;
    }

    for (size_t check_num = 0; check_num < prev.size; check_num++){
        if (prev.data[check_num] == el){
            return true;
        }
    }

    prev.data[prev.size] = el;
    prev.size++;

    if (prev.size == prev.capacity){
        prev.capacity *= 2;
        node_t **new_data = (node_t **)realloc(prev.data, prev.capacity * sizeof(node_t *));
        assert(new_data);

        prev.data = new_data;
    }

    return false;
}