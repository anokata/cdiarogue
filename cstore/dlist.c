#include "dlist.h"
void free_node_stumb(data_t ptr) {
    //printf("* Free at %p\n", ptr);
    //free(ptr);
}

char* data_to_str_stumb(data_t data) { // ALLOC
    char* str = malloc(sizeof(uint32_t) + 1);
    //sprintf(str, "%d", *((uint32_t*)data));
    return str;
}

void list_init(DList *list) {
    list->length = 0;
    list->head = 0;
    list->tail = 0;
    list->free_fnc = free_node_stumb;
    list->tostr_fnc = data_to_str_stumb;
    list->data_size = sizeof(data_t);
}

DList* list_new() {
    DList* list = malloc(sizeof(*list));
    list_init(list);
    return list;
}

data_t datai(uint32_t x) {
    data_t d;
    d.data_type = DATA_UINT32;
    d.val.idata = x;
    return d;
}

data_t datas(char* s) {
    data_t d;
    d.data_type = DATA_STR;
    d.val.sdata = s;
    return d;
}

DListNode* list_newnodei(uint32_t x) {
    return list_newnode(datai(x));
}

DListNode* list_newnodes(char *s) {
    return list_newnode(datas(s));
}

DListNode* list_newnode(data_t data) {
    DListNode* new_elem; 
    new_elem = malloc(sizeof(*new_elem));
    new_elem->data = data;
    new_elem->next = 0;
    new_elem->back = 0;
    return new_elem;
}

int list_push(DList* list, data_t data) {
    DListNode* ln = list->head;
    DListNode* new_elem = list_newnode(data);
    list->length++;
    if (ln == 0) {
        list->head = new_elem;
        list->tail = new_elem;
        return 0;
    }
    list->tail->next = new_elem;
    new_elem->back = list->tail;
    list->tail = new_elem;
    return 0;
}

int list_remove(DList* list) {
    if (list->length == 0) {
        return 0;
    }
    DListNode* old_head = list->head;
    list->head = list->head->next;
    if (list->head)
        list->head->back = 0;
    list->free_fnc(old_head->data);
    free(old_head);
    list->length--;
    if (list->length == 0) {
        list->tail = 0;
    }
    return 0;
}

int list_pop(DList* list) {
    DListNode* lst = list->head;
    if (list->length == 0) {
        return 0;
    }
    if (list->length == 1) {
        data_t data = list->head->data;
        list->free_fnc(list->head->data);
        free(list->head);
        list->head = 0;
        list->tail = 0;
        list->length--;
        return 0;
    } else {
        list->length--;
        lst = list->tail->back;
        data_t data = lst->next->data;
        list->free_fnc(lst->next->data);
        free(lst->next);
        lst->next = 0;
        list->tail = lst;
        return 0;
    }
}

int list_addi(DList* list, uint32_t x) {
    return list_add(list, datai(x));
}

int list_adds(DList* list, char *s) {
    return list_add(list, datas(s));
}

int list_add(DList* list, data_t data) {
    DListNode* new_elem = list_newnode(data);
    list->length++;

    if (list->head == 0) {
        list->head = new_elem;
        list->tail = new_elem;
        return 0;
    } else {
        new_elem->next = list->head;
        list->head->back = new_elem;
        list->head = new_elem;
        return 0;
    }
}

void list_map(DList* list, List_map_func f) {
    DListNode *ln = list->head;
    while (ln) {
        ln->data = f(ln->data);
        ln = ln->next;
    }
}

data_t list_print_func(data_t data) {
    //switch(data.data_type) {
    if (data.data_type == DATA_UINT32) {
        printf("int(%d)", data.val.idata);
    }
    if (data.data_type == DATA_STR) {
        printf("str(%s)", data.val.sdata);
    }
    return data;
}

void list_print(DList *list) {
    int i = 1;
    DListNode *ln = list->head;
    while (ln) {
        list_print_func(ln->data);
        printf("\t");
        printf("Item[%d] ", i++);
        printf("next: %p back: %p \t", ln->next, ln->back);
        ln = ln->next;
        printf("\n");
    }
    printf("\n");
}

void list_p(DList* list) {
    list_map(list, list_print_func);
}

DListNode* list_delete_node(DList* list, DListNode* ln) {
    DListNode* next = ln->next;
    DListNode* back = ln->back;
    if (next)
        next->back = back;
    if (back)
        back->next = next;
    if (ln == list->head)
        list->head = ln->next;
    if (ln == list->tail)
        list->tail = ln->back;
    list->free_fnc(ln->data);
    free(ln);
    return next;
}

void list_delete_onlynodes(DList* list) {
    DListNode* cur = list->head;
    while(cur) {
        DListNode* next = cur->next;
        list->free_fnc(cur->data);
        free(cur);
        cur = next;
    }
}

void list_delete(DList* list) {
    DListNode* cur = list->head;
    while(cur) {
        DListNode* next = cur->next;
        list->free_fnc(cur->data);
        free(cur);
        cur = next;
    }
    list->length = 0;
    list->head = 0;
    list->tail = 0;
    free(list);
}

const static uint32_t LIST_TAG = 'D' + ('L' << 8) + ('S' << 16) + ('T' << 24);
const static char DLIST_TAG[] = "DLST";

int list_save2file(DList* list, char* fn) {
	/*FILE *fd = fopen(fn, "w");
	if (!fd) {
		perror("open");
		return -1;
	}
    // TODO first serialize to memory, after write once
	size_t bytes = fwrite(&LIST_TAG, sizeof(LIST_TAG), 1, fd);
    DListNode* cur = list->head;
    bytes += fwrite(&list->length, sizeof(uint32_t), 1, fd);
    bytes += fwrite(&list->data_size, sizeof(uint32_t), 1, fd);
    while (cur) {
        // TODO convert data to serialized
        bytes += fwrite(cur->data, list->data_size, 1, fd);
        cur = cur->next;
    }
	printf("* writed %ld items\n", bytes);
	fclose(fd);
    */
	return 0;
}

DList *list_loadfile(char* fn) {
    /*
    FILE *fd = fopen(fn, "r");
    if (!fd) {
        perror("open for load");
        return 0;
    }
    DList *list = list_new();
    // check tag
    uint32_t tag;
    size_t readed = fread(&tag, sizeof(tag), 1, fd);
    if (!readed) {
        perror("not read");
        return 0;
    }
    if (tag != LIST_TAG) {
        perror("Tag not is LIST");
        return 0;
    }
    uint32_t length;
    uint32_t size;
    void *data;
    fread(&length, sizeof(list->length), 1, fd);
    fread(&size, sizeof(list->data_size), 1, fd);
    list->data_size = size;
    for (size_t i = 0; i < length; i++) {
        data = malloc(size);
        fread(data, size, 1, fd);
        list_push(list, data);
    }

    return list;
    */
}

void *list_free_data(void *data) {
    free(data);
    return 0;
}

int list_erase_at(DList *list, uint32_t index) {
    if (list->length < index || index < 0) return -1;
    DListNode *cur = list->head;
    while(index--) {
        cur = cur->next;
    }
    list_delete_node(list, cur);
    list->length--;
    return 0;
}

/*
   #LISTTAG|COUNT
   #TYPETAG|VALUE

   DLST|3
   STRN|some stirng
   I32|7892
   DBL|3.14

   or
   s|str
   I|12
   f|3.1

 */

#define DELIMITER '|'
#define ENDLINE '\n'
static const char DATA_UINT32_TAG[] = "I32";
static const char DATA_STR_TAG[] = "STR";

char *data_to_str(char *buf, size_t maxs, data_t data) {
    switch (data.data_type) {
        case DATA_UINT32: 
            sprintf(buf, "%s%c%d\n", DATA_UINT32_TAG, DELIMITER, data.val.idata);
            break;
        case DATA_STR:
            snprintf(buf, maxs, "%s%c%s\n", DATA_STR_TAG, DELIMITER, data.val.sdata);
            break;
    };
    return buf;
}
#define BUF_DSV_SIZE 128
char *list_get_dsvstr(DList *list) {
    char buf[BUF_DSV_SIZE];
    snprintf(buf, BUF_DSV_SIZE, "%s%c%d\n", DLIST_TAG, DELIMITER, list->length);
    char *res = calloc(strlen(buf) + 1, 1);
    char *d;
    strcpy(res, buf);

    DListNode *cur = list->head;
    size_t len = strlen(res) + 1;
    while (cur) {
        // TODO refactor to realloc
        data_to_str(buf, BUF_DSV_SIZE, cur->data);
        d = strdup(res);
        len += strlen(buf) + 1;
        free(res);
        res = calloc(len, 1);
        strcat(res, d);
        strcat(res, buf);
        cur = cur->next;
    }
    return res;
}
//TODO map, print on List contained other list
//
char *substr(char *s, uint32_t start, uint32_t end) {
//TODO
    //char *result = malloc(
}

DList *list_from_dsvstr(char *s) {
    // TODO
    if (strncmp(s, DLIST_TAG, strlen(DLIST_TAG)) == 0) {
        DList *list = list_new();
        char buf[BUF_DSV_SIZE];
        char *lenstr;
        char* dpos = strchr(s, DELIMITER) + 1;
        char* npos = strchr(dpos, ENDLINE);
        strncpy(buf, dpos, npos - dpos);
        size_t len = atoi(buf);
        dpos++;
        printf("len=%ld %s\n", len, dpos);
        // for (size_t i = 0; i < len; i++) {
        // 
        dpos = strchr(npos, DELIMITER) + 1;
        printf("type(%s)\n", npos + 1);
        if (strncmp(npos + 1, DATA_UINT32_TAG, strlen(DATA_UINT32_TAG)) == 0) {
            npos = strchr(dpos, ENDLINE);
            strncpy(buf, dpos, npos - dpos);
            buf[npos - dpos] = 0;
            printf("NUMBER val(%s)\n", buf);
        }
        if (strncmp(npos + 1, DATA_STR_TAG, strlen(DATA_STR_TAG)) == 0) {
            printf("STRing\n");
        }
        return list;
    }
    return 0;
}

int list_saveDSVfd(DList* list, FILE* fd) {
}

int list_saveDSVfile(DList* list, char* fn) {

}

// save/load separator data, text format DSV with | delimeter
// save(func to save one elem(fd))
// push pop append remove take head tail slice reduce
// delete delete_map
// get erase insert
// save/load serialization
// make text format with sep
//#define DEBUG
#ifdef DEBUG
#include "dlist_test.c"
#endif
