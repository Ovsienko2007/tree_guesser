#include "tree.h"

#define COL_NEXT_BACKGROUND    "#7df879ff"
#define COL_LEFT_BACKGROUND    "#f879e7ff" 
#define COL_RIGHT_ARROW        "#0ea30eff"
#define COL_VALUE_BACKGROUND   "#7f8cd6be"
#define COL_CURRENT_BACKGROUND "#d3cd77ff"

static FILE *html_stream = nullptr;

static const char *dump_file_position  = "DUMP/dump.html";
static const int kMaxFileNameLen       = 24;
static const int kImageSize            = 500;

static void creat_html(int num_call, dump_position position);
static void creat_dot(int num_call, tree_t tree);
static void print_dump_elem(FILE *strem_out, node_t *elem);

static void start_dump();
static void end_dump();

void show_dump(tree_t tree, dump_position position){
    static int num_call = 1;

    if (num_call == 1){
        start_dump();
        atexit(end_dump);
    }

    creat_dot(num_call, tree);
    creat_html(num_call, position);
    num_call++;
}

static void creat_html(int num_call, dump_position position){
    if (!html_stream) return;

    fprintf(html_stream, "<h3 align=\"center\"> StackDump called from %s:%d from func %s. The %d call</h3>\n",
         position.file, position.line, position.func, num_call);

    fprintf(html_stream,  "<img src=\"./%d.png\" width=\"%d\" alt=\"DUMP %d\" />\n", num_call, kImageSize, num_call);
}

static void creat_dot(int num_call, tree_t tree){
    char file_name[kMaxFileNameLen] = {};
    sprintf(file_name, "DUMP/%d.dot", num_call);

    FILE *strem_out = fopen(file_name ,"w");
    assert(strem_out);
    fprintf(strem_out, "digraph structs {\n");
    if (tree){
        print_dump_elem(strem_out, tree);
    }
    fprintf(strem_out, "}\n");
    fclose(strem_out);
}

static void print_dump_elem(FILE *strem_out, node_t *elem){
    assert(elem);

    fprintf(strem_out, 
        "%lu [shape=\"plaintext\", label=<\n"
        "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n", (unsigned long)elem);
    fprintf(strem_out, "<TR><TD BGCOLOR=\"" COL_CURRENT_BACKGROUND "\" COLSPAN=\"2\">%p</TD></TR>\n", elem);
    fprintf(strem_out, "<TR><TD BGCOLOR=\"" COL_VALUE_BACKGROUND   "\" COLSPAN=\"2\">%s</TD></TR>\n", elem->value);
        
    fprintf(strem_out,
        "    <TR>\n"
        "        <TD BGCOLOR=\"" COL_LEFT_BACKGROUND    "\"> %p </TD>\n"
        "        <TD BGCOLOR=\"" COL_NEXT_BACKGROUND    "\"> %p </TD>\n"
        "    </TR>\n"
        "</TABLE>\n"
        ">];\n", elem->ans1, elem->ans2);
    
    if (elem->ans1){
        fprintf(strem_out, "%lu->%lu[color=\"" COL_RIGHT_ARROW "\"];\n", (unsigned long)elem, (unsigned long)elem->ans1);
        print_dump_elem(strem_out, elem->ans1);
    }
    if (elem->ans2){
        fprintf(strem_out, "%lu->%lu[color=\"" COL_RIGHT_ARROW "\"];\n", (unsigned long)elem, (unsigned long)elem->ans2);
        print_dump_elem(strem_out, elem->ans2);
    }
}

static void start_dump(){
    html_stream = fopen(dump_file_position,"w");
    fprintf(html_stream,  "<html lang=\"en\">\n"
                        "<head>\n"
                        "  <meta charset=\"UTF-8\">\n"
                        "  <title>Dump HTML</title>\n"
                        "</head>\n"
                        "<body>\n");
}   

static void end_dump(){
    if (!html_stream) return;

    fprintf(html_stream,  "</body>\n"
                        "</html>\n");
                        
    fclose(html_stream);
}