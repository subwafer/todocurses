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

void read_from_file(const char *file_path, Todos *todos);

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

int main(int argc, char **argv) {
    handle_cli_args(argc, argv);

    Todos todos;

    todos.file_path = "./examples/todos";
    read_from_file(todos.file_path, &todos); // why path file_path separatly? Maybe it's worth passing each param needed from struct separately?

    if (todos.todos_content == NULL) {
        printf("TODOS_CONTENT == NULL\n");
    }

    // final clean up
    for (int i = 0; i < todos.count; i++) {
        free(todos.todos_content[i]);
    }
    free(todos.todos_content);

    return 0;
}

void read_from_file(const char *file_path, Todos *todos) {
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

    todos->count = 0;

    todos->todos_content = malloc(sizeof(char*));

    // so in this loop, we dynamically allocate memory for a variable num of todos.
    while (fgets(buffer, cs, file) != NULL) {
        // TODO: Error handling?
        char *t = malloc(strlen(buffer) + 1);

        strcpy(t, buffer);

        todos->todos_content = realloc(todos->todos_content, (todos->count + 1) * sizeof(char*));

        todos->todos_content[todos->count] = t;

        todos->count++;
    }

    printf("Total TODOs: %d\n", todos->count);

    todos->content_size = cs;

cleanup:
    if (file) fclose(file);
    if (buffer) free(buffer);
}
