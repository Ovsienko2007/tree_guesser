#include "tree.h"
static bool get_answer();
static void clean_buffer();

void destroy_tree(tree_t *tree) {
    assert(tree);
    if (!tree) return;
    if (!*tree) return;

    if ((*tree)->ans1) {
        destroy_tree(&((*tree)->ans1));
    }
    if ((*tree)->ans2) {
        destroy_tree(&((*tree)->ans2));
    }
    free(*tree);
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

void check_elem(tree_t tree){
    assert(tree);
    if (tree->ans1 == nullptr && tree->ans2 == nullptr){
        printf("Ответ: %s? (Да/Нет)\n", tree->value);

        if (!get_answer()){
            printf("Хотите добавить новый элемент? (Да/Нет)\n");

            if (get_answer()) add_elem(tree);
        }
    }
    else{
        printf("То что вы загадали %s? (Да/Нет)\n", tree->value);

        if (get_answer()){
            return check_elem(tree->ans1);
        }
        else{
            return check_elem(tree->ans2);
        }
    }
}

tree_t init_tree(){
    char *value = (char *)calloc(kMaxAnsLen, sizeof(char));
    strcpy(value, "Неизветсно что");

    tree_t tree = init_node();
    tree->value = value;

    return tree;
}

void add_elem(node_t *node){
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
    fprintf(stderr, "%s\n", user_answer2);
    node->value = user_answer2;
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