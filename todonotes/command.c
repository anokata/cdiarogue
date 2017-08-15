#include "command.h"
#include "notesbook.h"
#include "util.h"
#define UNUSED(x) ((void)(x))

char *enter_note();

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

char *_get_bookname(char **params) {
    char *bookname = params[0];
    if (bookname == NULL) {
        printf("Need more parametrs\n");
        exit(6);
    }
    return bookname;
}

void print_nbook(char **params) {
    NotesBook book = nbook_load(_get_bookname(params));
    nbook_print(book);
    nbook_free(book);
}

void cmd_book(char **params) {
    char *bookname = _get_bookname(params);
    nbook_file_new(bookname);
}

void cmd_unbook(char **params) {
    char *bookname = _get_bookname(params);
    nbook_file_rm(bookname);
}

// FIXME empty string
char *str_trim_nl(char *text) {
    size_t len = strlen(text) - 1;
    char *end = text + len;
    while (len-- && *end == '\n') {
        *end = '\0';
        end--;
    }
    return text;
}

char *enter_note() {
    size_t len = 1;
    const size_t max_line_len = 80;
    char *text = malloc(max_line_len);
    memset(text, 0 , max_line_len);
    char *buf = malloc(max_line_len);

    while (fgets(buf, max_line_len, stdin) != NULL) {
        len += strlen(buf);
        text = realloc(text, 1 + len);
        strcat(text, buf);
    }
    free(buf);
    str_trim_nl(text);
    return text;
}

void cmd_note(char **params) {
    char *bookname = _get_bookname(params);
    char *notename = _get_bookname(params + 1);
    printf("Creating note %s in book %s \n", notename, bookname);
    // load book, with check
    NotesBook book = nbook_load(bookname);
    printf("Book loaded ok, contains %d notes.\n", nbook_len(book));
    // Read stdin for text
    char *text = enter_note();
    // create note, add, store, close book
    Note note = note_new(g_strdup(notename), text);
    nbook_add_note(book, note);
    nbook_save(book);
    nbook_free(book);
}

void cmd_remove(char **params) {
    // by name or by index? by title not 
    char *bookname = _get_bookname(params);
    char *noteindex = _get_bookname(params + 1);
    printf("Removing note #%s in book [%s] \n", noteindex, bookname);
    // to int
    int note_index = atoi(noteindex);
    // 1. check contains digits
    size_t digits = strspn(noteindex, "0123456789");
    // TODO check result
    // or strtol
    
    NotesBook book = nbook_load(bookname);
    printf("Book loaded ok, contains %d notes.\n", nbook_len(book));
    // nbook_note_remove(book, note_index)

    nbook_save(book);
    nbook_free(book);
}

Command Commands[] = {
    {"help", print_help},
    {"list", print_list},
    {"show", print_nbook},
    {"book", cmd_book},
    {"unbook", cmd_unbook},
    {"note", cmd_note},
    {"rm", cmd_remove},
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


