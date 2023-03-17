#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

bool DEBUG_MODE = false;

typedef struct {
    char *file_path;
    int count;
    size_t content_size;
    char **todos_content;
} Todos;

void read_from_file(const char *file_path, Todos *t);

void handle_cli_args(int argc, char **argv) {
    if (argc > 0) {
        for (int i = 0; i < argc; i++) {
            if (strcmp(argv[i], "--debug") == 0) {
                DEBUG_MODE = true;
                printf("-------> DEBUG MODE ACTIVE <-------\n");
            }
        }
    }
}

void debug_print_info(const Todos *t) {
    if (DEBUG_MODE == true) {
        printf("------START DEBUG INFO------\n");
        printf("Total TODOs: %d\n", t->count);
        for (int i = 0; i < t->count; i++) {
            printf("[TODO #%d]: %s", i + 1, t->todos_content[i]);
        }
        printf("------END DEBUG INFO--------\n");
    }
}

int main(int argc, char **argv) {
    handle_cli_args(argc, argv);

    Todos todos;

    todos.file_path = "./examples/todos";
    read_from_file(todos.file_path, &todos); // why path file_path separatly? Maybe it's worth passing each param needed from struct separately?

    debug_print_info(&todos);

    // final clean up
    for (int i = 0; i < todos.count; i++) {
        free(todos.todos_content[i]);
    }
    free(todos.todos_content);

    return 0;
}

void read_from_file(const char *file_path, Todos *t) {
    char *buffer = {0};

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Couldn't open file %s\n", file_path); //TODO: add better errors
        goto cleanup;
    }

    if (fseek(file, 0, SEEK_END) > 0) {
        fprintf(stderr, "ERROR: Couldn't seek to end of file %s\n", file_path); //TODO: add better errors
        goto cleanup;
    }

    long cs = ftell(file);

    if (fseek(file, 0, SEEK_SET) > 0) {
        fprintf(stderr, "ERROR: Couldn't seek to start of file %s\n", file_path); //TODO: add better errors
        goto cleanup;
    }

    buffer = malloc(cs * sizeof(char));

    t->count = 0;

    t->todos_content = malloc(sizeof(char*));

    // so in this loop, we dynamically allocate memory for a variable num of todos.
    while (fgets(buffer, cs, file) != NULL) {
        // TODO: Error handling?
        char *at = malloc(strlen(buffer) + 1);

        strcpy(at, buffer);

        t->todos_content = realloc(t->todos_content, (t->count + 1) * sizeof(char*));

        t->todos_content[t->count] = at;

        t->count++;
    }


    t->content_size = cs;

cleanup:
    if (file) fclose(file);
    if (buffer) free(buffer);
}
