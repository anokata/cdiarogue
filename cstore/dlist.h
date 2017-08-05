#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
// Double linked list
typedef union data_u {
    char *sdata;
    uint32_t idata;
} data_u;

typedef enum edata_type {DATA_UINT32, DATA_STR} edata_type;

typedef struct data_t {
    edata_type data_type;
    data_u val;
} data_t;


typedef void (*free_node_fnc)(data_t);
typedef data_t (*List_map_func)(data_t);
typedef char* (*data_to_str_fnc)(data_t);

typedef struct DListNode { 
    struct DListNode* next;
    struct DListNode* back;
    data_t data;
} DListNode;

typedef struct DList {
    DListNode* head;
    DListNode* tail;
    uint32_t length;
    uint32_t data_size;
    free_node_fnc free_fnc;
    data_to_str_fnc tostr_fnc;
} DList;

DListNode* list_newnode(data_t data);
DList* list_new();
void list_p(DList* list);
void list_print(DList *list);
void list_map(DList* list, List_map_func f);
data_t list_print_func(data_t data);
int list_pop(DList* list);
int list_push(DList* list, data_t data);
int list_add(DList* list, data_t data);
int list_remove(DList* list);
void list_delete(DList* list);
void list_delete_onlynodes(DList* list);
void list_init(DList *list);
int list_erase_at(DList *list, uint32_t index);
DListNode* list_newnodes(char *s);
DListNode* list_newnodei(uint32_t x);
data_t datas(char* s);
data_t datai(uint32_t x);
int list_addi(DList* list, uint32_t x);
int list_adds(DList* list, char *s);
char *list_get_dsvstr(DList *list);
// TODO
// find

