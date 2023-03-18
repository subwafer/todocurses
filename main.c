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
void create_todo(char *todo_body, Todos *t);
void edit_todo(int t_index, char *todo_body, Todos *t);
void archive_todo(void);
void save_to_file(void);

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

void debug_auto_test(Todos *t) {
    // this will run tests on the funcs. and display the outputs to stdout.
    // probably worth pulling out into it's own file.

    if (DEBUG_MODE == true) {
        printf("------START DEBUG AUTO TEST------\n");

        t->file_path = "./examples/todos";
        printf("TEST: Reading file: %s\n\n", t->file_path);
        read_from_file(t->file_path, t); // why path file_path separatly? Maybe it's worth passing each param needed from struct separately?

        printf("t->count: %d\n", t->count);
        printf("t->content_size: %zu\n", t->content_size);

        printf("----------------------------------------------------\n");
        printf("TEST: Total TODOs read in from file: %d\n", t->count);
        for (int i = 0; i < t->count; i++) {
            printf("[TODO #%d]: %s", i + 1, t->todos_content[i]);
        }
        printf("----------------------------------------------------\n");
        printf("\n");

        char *debug_test_todo = "here is a test todo\n";
        printf("TEST: Creating todo: %s\n", debug_test_todo);
        create_todo(debug_test_todo, t);
        printf("----------------------------------------------------\n");
        printf("TEST: Created TODO appended to file: %d\n", t->count);
        for (int i = 0; i < t->count; i++) {
            printf("[TODO #%d]: %s", i + 1, t->todos_content[i]);
        }
        printf("----------------------------------------------------\n");
        printf("\n");

        int t_index = 0;
        char *debug_test_edit_todo = "this todo has been edited by debug_test\n";
        printf("TEST: Editing todo #%d | -> %s\n", t_index, debug_test_edit_todo);
        edit_todo(t_index, debug_test_edit_todo, t);
        printf("----------------------------------------------------\n");
        printf("TEST: Created TODO appended to file: %d\n", t->count);
        for (int i = 0; i < t->count; i++) {
            printf("[TODO #%d]: %s", i + 1, t->todos_content[i]);
        }
        printf("----------------------------------------------------\n");

        printf("\n");
        printf("------END DEBUG AUTO TEST--------\n");
    }

    // if we call this, we just exit the program.
    exit(0);
}

int main(int argc, char **argv) {
    handle_cli_args(argc, argv);

    Todos todos;

    debug_auto_test(&todos);

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

void create_todo(char *todo_body, Todos *t) {
    // TODO: Add more logging and error handling
    FILE *f = fopen(t->file_path, "a");
    if (f == NULL) {
        fprintf(stderr, "ERROR create_todo: Could not open file %s\n", t->file_path);
        goto cleanup;
    }

    // add to t->todos_content buffer
    t->todos_content = realloc(t->todos_content, (t->count + 1) * sizeof(char*));

    t->todos_content[t->count] = todo_body;

    t->count++;

    fwrite(todo_body, 1, strlen(todo_body), f);

cleanup:
    fclose(f);
}

void edit_todo(int t_index, char *todo_body, Todos *t) {
    FILE *f = fopen(t->file_path, "w");
    if (f == NULL) {
        fprintf(stderr, "ERROR edit_todo: Could not open file %s\n", t->file_path);
        goto cleanup;
    }

    // TODO: I believe I should update the ->content_size.
    // After updating, creating, etc. The t->content_size is no longer valid
    // until it reads the file again (program re-execution).
    // I could technically just reread the file again to update the size?
    // Or I could just do that here (and in other functions, edit, etc.)

    t->todos_content[t_index][0] = 0;

    t->todos_content[t_index] = realloc(t->todos_content[t_index], (strlen(todo_body) + 1) * sizeof(char));

    strcpy(t->todos_content[t_index], todo_body);

    for (int i = 0; i < t->count; i++) {
        fputs(t->todos_content[i], f);
    }

cleanup:
    if (f) fclose(f);
}

void archive_todo(void) {
    // TODO: Implement archive_todo
    // Idea: make an archive struct. This can hold the index of the todo, the contents
    // if the user decides to "unarchive", we can then easily place it back into the correct index of Todos.todos_content
    // store the archive buffer into a file for persistence
    // load the archive on launch
    // create dir + file if doesn't exist, etc.
    printf("archive_todo not implemented yet.\n");
}

void save_to_file(void) {
    // TODO: Implement a save to file function
    // after creating, editing, deleting, etc. Save the todos to the file
    printf("save_to_file not implemented.\n");
}
