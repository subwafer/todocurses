#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void read_from_file(char *file_path);

bool DEBUG_MODE = false;

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

    char *file_path = "./examples/todos";
    read_from_file(file_path);

    return 0;
}

void read_from_file(char *file_path) {
   (void) file_path;

   printf("read_from_file not implemented.\n");
}
