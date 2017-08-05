#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef void (*free_node_fnc)(void*);
typedef char* (*data_to_str_fnc)(void*);

typedef struct ListNode { // Node
    struct ListNode* next;
    void* data;
} ListNode;

typedef struct List {
    ListNode* head;
    ListNode* tail;
    uint32_t length;
    uint32_t data_size;
    free_node_fnc free_fnc;
    data_to_str_fnc tostr_fnc;
} List;

typedef void* (*List_map_func)(void*);
ListNode* list_newnode(void* data);
List* list_new();
void* list_pop(List* list);
void list_p(List* list);
void list_print(List *list);
void list_map(List* list, List_map_func f);
void* list_print_func(void* data);
void* list_pop(List* list);
int list_push(List* list, void* data);
int list_add(List* list, void* data);
int list_remove(List* list);

void free_node_stumb(void* ptr) {
    printf("* Free at %p\n", ptr);
}

char* data_to_str_stumb(void* data) { // ALLOC
    char* str = malloc(sizeof(uint32_t) + 1);
    sprintf(str, "%d", *((uint32_t*)data));
    return str;
}

List* list_new() {
    List* list = malloc(sizeof(*list));
    list->length = 0;
    list->head = 0;
    list->tail = 0;
    list->free_fnc = free_node_stumb;
    list->tostr_fnc = data_to_str_stumb;
    list->data_size = sizeof(uint32_t);
    return list;
}

ListNode* list_newnode(void* data) {
    ListNode* new_elem; 
    new_elem = malloc(sizeof(*new_elem));
    new_elem->data = data;
    new_elem->next = 0;
    return new_elem;
}

int list_push(List* list, void* data) {
    ListNode* ln = list->head;
    ListNode* new_elem = list_newnode(data);
    list->length++;
    if (ln == 0) {
        list->head = new_elem;
        return 0;
    }
    while (ln->next) {
        ln = ln->next;
    }
    ln->next = new_elem;
    list->tail = new_elem;
    return 0;
}

int list_remove(List* list) {
    if (list->length == 0) {
        return 0;
    }
    ListNode* old_head = list->head;
    list->head = list->head->next;
    list->free_fnc(old_head->data);
    free(old_head);
    list->length--;
    if (list->length == 0) {
        list->tail = 0;
    }
    return 0;
}

void* list_pop(List* list) {
    ListNode* lst = list->head;
    if (list->length == 0) {
        return 0;
    }
    if (list->length == 1) {
        list->free_fnc(list->head->data);
        free(list->head);
        void* data = lst->data;
        list->head = 0;
        list->tail = 0;
        list->length--;
        return data;
    } else {
        list->length--;
        while (lst->next->next != 0) {
            lst = lst->next;
        }
        void* data = lst->next->data;
        list->free_fnc(lst->next->data);
        free(lst->next);
        lst->next = 0;
        list->tail = lst;
        return data;
    }
}

int list_add(List* list, void* data) {
    ListNode* new_elem = list_newnode(data);
    list->length++;

    if (list->head == 0) {
        list->head = new_elem;
        list->tail = new_elem;
        return 0;
    } else {
        new_elem->next = list->head;
        list->head = new_elem;
        return 0;
    }
}

void list_map(List* list, List_map_func f) {
    ListNode *ln = list->head;
    while (ln) {
        ln->data = f(ln->data);
        ln = ln->next;
    }
}

void* list_print_func(void* data) {
    printf("data: %p int(%d) long(%ld) ", data, *(int*)data, *(long*)data);
    printf("str(%s) \n", (char*)data);
    return data;
}

void list_print(List *list) {
    int i = 1;
    ListNode *ln = list->head;
    while (ln) {
        printf("Item[%d] ", i++);
        printf("next: %p  ", ln->next);
        printf("data: %p int(%d) long(%ld) ", ln->data, *(int*)ln->data, *(long*)ln->data);
        printf("str(%s) \n", (char*)ln->data);
        ln = ln->next;
    }
    printf("\n");
}

void list_p(List* list) {
    list_map(list, list_print_func);
}

ListNode* list_delete_node(List* list, ListNode* ln) {
    ListNode* next = ln->next;
    list->free_fnc(ln->data);
    free(ln);
    return next;
}

void list_delete(List* list) {
    ListNode* cur = list->head;
    while(cur) {
        ListNode* next = cur->next;
        list->free_fnc(cur->data);
        free(cur);
        cur = next;
    }
    list->length = 0;
    list->head = 0;
    list->tail = 0;
}

const static uint32_t LIST_MAGIC = 
    0b01110000 | 
    0b00100000 << 8| 
    0b00100100 << 16| 
    0b00111100 << 24;
const static uint32_t LIST_TAG = 'L' + ('I' << 8) + ('S' << 16) + ('T' << 24);

int list_save2file(List* list, char* fn) {
	FILE *fd = fopen(fn, "w");
	if (!fd) {
		perror("open");
		return -1;
	}
    // TODO first serialize to memory, after write once
	size_t bytes = fwrite(&LIST_TAG, sizeof(LIST_TAG), 1, fd);
    ListNode* cur = list->head;
    bytes += fwrite(&list->length, sizeof(uint32_t), 1, fd);
    bytes += fwrite(&list->data_size, sizeof(uint32_t), 1, fd);
    while (cur) {
        // TODO convert data to serialized
        bytes += fwrite(cur->data, list->data_size, 1, fd);
        cur = cur->next;
    }
	printf("* writed %ld items\n", bytes);
	fclose(fd);
	return 0;
}

List *list_loadfile(char* fn) {
    FILE *fd = fopen(fn, "r");
    if (!fd) {
        perror("open for load");
        return 0;
    }
    List *list = list_new();
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
}

// save(func to save one elem(fd))
// push pop append remove take head tail slice reduce
// delete delete_map
// get erase insert
// save/load serialization
#define DEBUG

#ifdef DEBUG
#include <string.h>
void test() {
    //separate test one func
    List *l; 
    int a = 812;
    char s[] = "Linked List.";
    l = list_new();
    list_add(l, &a);
    list_add(l, &s);
    printf("%d\n", *(int*)l->head->next->data);
    list_add(l, &s);
    printf("%s\n", (char*)l->head->data);
    list_print(l);
    list_p(l);
    int b = 3; int c = 4;
    list_delete(l);
    l = list_new();
    list_push(l, &a);
    list_push(l, &b);
    list_push(l, &c);
    list_p(l);
    int x1 = 1;
    int x2 = 2;
    int x3 = 3;
    int d = 5;
    list_delete(l);
    l = list_new();
    list_push(l, &x1);
    list_push(l, &x2);
    list_push(l, &x3);
    list_add(l, &c);
    list_add(l, &d);
    printf("pop = %d len(%d)\n", *(int*)list_pop(l), l->length);
    printf("pop = %d len(%d)\n", *(int*)list_pop(l), l->length);
    printf("pop = %d len(%d)\n", *(int*)list_pop(l), l->length);
    printf("h = %d\n", *(int*)l->head->data);
    printf("t = %d\n", *(int*)l->tail->data);
    printf("xh = %p\n", (int*)l->head);
    printf("xt = %p\n", (int*)l->tail);
    printf("pop = %d len(%d)\n", *(int*)list_pop(l), l->length);
    printf("h = %p\n", (int*)l->head);
    printf("t = %p\n", (int*)l->tail);
    printf("\n");
    list_p(l);
    list_delete(l);
    l = list_new();
    list_push(l, &x1);
    list_push(l, &x2);
    list_push(l, &x3);
    list_add(l, &c);
    list_add(l, &d);
    list_remove(l);
    printf("pop = %d len(%d)\n", *(int*)list_pop(l), l->length);
    list_remove(l);
    list_p(l);
    list_delete(l);
    list_p(l);
}

void test_save() {
    int a = 1; int b = 2;
    List* l = list_new();
    list_push(l, &a);
    list_add(l, &b);
    list_save2file(l, "testsavelist");
    list_remove(l);
}

void test_load() {
    List *l = list_loadfile("testsavelist");
    printf("loaded %d items\n", l->length);
    list_p(l);
    list_remove(l);
}

int main() {
    //test();
    //test_save();
    test_load();
    return 0;
}
#endif
