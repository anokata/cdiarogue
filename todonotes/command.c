#include "command.h"
#include "notesbook.h"
#define UNUSED(x) ((void)(x))

void print_help(char **params) {
    UNUSED(params);
    printf("Params: %s\n", params[0]);
    printf("help is\n");
}

void print_list(char **params) {
    UNUSED(params);
    UNUSED(notebooks_path);

    nbook_list();
}

void print_nbook(char **params) {
    char *bookname = params[0];
    if (bookname == NULL) {
        printf("no parametr\n");
        exit(6);
    }
    NotesBook book = nbook_load(bookname);
    nbook_print(book);
    nbook_free(book);
}

Command Commands[] = {
    {"help", print_help},
    {"list", print_list},
    {"show", print_nbook},
    {"book", print_help},
};

static int commands_count = sizeof(Commands)/sizeof(Command);

Command cmd_get(char *name) {
    for (int i = 0; i < commands_count; i++) {
        if (strcmp(Commands[i].name, name) == 0) {
            return Commands[i];
        }
    }
    return Commands[0];
}


