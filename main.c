#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

bool DEBUG_MODE = false;

int create_todo(char *file_path, char *todo_content);
char *read_from_file(char *file_path, size_t *content_size);

int main(int argc, char **argv) {
    if (argc > 0) {
        for (int i = 0; i < argc; i++) {
            if (strcmp(argv[i], "--debug") == 0) {
                DEBUG_MODE = true;
                printf("------------> DEBUG MODE ACTIVE <------------\n");
            }
        }
    }

    char *file_path = {0};

    if (DEBUG_MODE) {
        file_path = "./examples/todos";
    } else {
        puts("Please enable --debug mode to continue.\n");
        exit(1);
    }

    size_t content_size = 0;
    char *todos = read_from_file(file_path, &content_size);
    fwrite(todos, 1, content_size, stdout);


    /*
    ** We should probably hold todos in an internal buffer
    ** then at the end of the program exe, write out to file once.
    ** struct Todo
    ** struct Todos
    **
    ** todo struct will include all data that make up a todo or help handle a todo
    ** todos struct will hold multiple Todos, the amount, size, etc.
    **
    ** this program will support cli mode + a ncurses gui mode (like gdb)
     */


    // create_todo
    char *test_todo = "test todo\n";
    create_todo(file_path, test_todo);

    todos = read_from_file(file_path, &content_size);

    printf("--------------------\n");
    fwrite(todos, 1, content_size, stdout);

    // delete_todo
    // edit_todo


    if (todos) free(todos);
    return 0;
}

int create_todo(char *file_path, char *todo_content) {
    // TODO: Instead of appending directly to the file, append to an internal buffer.

    FILE *file = fopen(file_path, "a");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Couldn't open file (w): %s\n", file_path);
        goto error;
    }

    fwrite(todo_content, 1, strlen(todo_content), file);

error:
    if (file) fclose(file);

    return 1;
}

char *read_from_file(char *file_path, size_t *content_size) {
    char *buffer = {0};

    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("ERROR: Couldn't open file %s\n", file_path);
        goto error;
    }

    if (fseek(file, 0, SEEK_END) > 0) {
        printf("ERROR: Couldn't get to end of file %s\n", file_path);
        goto error;
    }

    long cs = ftell(file);

    if (fseek(file, 0, SEEK_SET) > 0) {
        printf("ERROR: Couldn't get to start of file %s\n", file_path);
        goto error;
    }

    // TODO: grab each todo by \n and add to a 2d array of todos
    buffer = malloc(cs * sizeof(char));

    size_t c = fread(buffer, 1, cs, file);
    assert(c == (size_t) cs);

    if (content_size) {
        *content_size = cs;
    }

    fclose(file);

    return buffer;

error:
    if (file) fclose(file);
    if (buffer) free(buffer);

    return NULL;
}
