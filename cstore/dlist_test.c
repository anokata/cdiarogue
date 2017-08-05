#include <string.h>
#include <assert.h>
void test_create() {
    printf("* TEST create \n");
    DList *l; 
    l = list_new();
    l->head = 0;
    list_delete(l);
}
void test_create_and_remove() {
    printf("* TEST create and remove one \n");
    DList *l; 
    l = list_new();
    list_remove(l);
    list_delete(l);
}
void test_create_and_delete() {
    printf("* TEST create and delete all\n");
    DList *l; 
    l = list_new();
    list_delete(l);
}
void test_add() {
    printf("* TEST ADD\n");
    DList *l = list_new();
    list_add(l, datai(1));
    list_add(l, datai(2));
    printf("Len:%d\tH:%p\tT:%p\n", l->length, l->head, l->tail);
    //printf("Head next %p\t back %p\t data %p(%d)\n", l->head->next, l->head->back, l->head->data, *(int*)l->head->data);
    list_print(l);
    list_remove(l);
    list_print(l);
    list_add(l, datai(3));
    list_print(l);
    list_delete(l);
}
void test_push() {
    printf("* TEST PUSH\n");
    DList *l = list_new();
    list_push(l, datai(1));
    list_push(l, datai(1));
    printf("Len:%d\tH:%p\tT:%p\n", l->length, l->head, l->tail);
    //printf("Head next %p\t back %p\t data %p(%d)\n", l->head->next, l->head->back, l->head->data, *(int*)l->head->data);
    list_print(l);
    list_remove(l);
    list_print(l);
    list_push(l, datai(1));
    list_print(l);
    list_delete(l);
}
void test_pop() {
    printf("* TEST POP\n");
    DList *l = list_new();
    list_push(l, datai(2));
    list_push(l, datai(2));
    printf("Len:%d\tH:%p\tT:%p\n", l->length, l->head, l->tail);
    //printf("Head next %p\t back %p\t data %p(%d)\n", l->head->next, l->head->back, l->head->data, *(int*)l->head->data);
    list_print(l);
    int x = list_pop(l);
    printf("poped: %d\n", x);
    list_print(l);
    list_push(l, datai(2));
    list_print(l);
    x = list_pop(l);
    printf("poped: %d\n", x);
    x = list_pop(l);
    printf("poped: %d\n", x);
    list_print(l);
    list_push(l, datai(2));
    list_add(l, datai(2));
    list_print(l);
    list_delete(l);
}
void test_backs() {
    printf("* TEST BACKS\n");
    DList *l = list_new();
    list_push(l, datai(8));
    list_push(l, datai(8));
    DListNode* c = l->tail;
    while (c) {
        //printf("self %p\tnext %p\t back %p\t data %p(%d)\n", c, c->next, c->back, c->data, *(int*)c->data);
        c = c->back;
    }
    list_print(l);
    list_remove(l);
    list_print(l);
    list_push(l, datai(1));
    list_print(l);
    c = l->tail;
    while (c) {
        //printf("self %p\tnext %p\t back %p\t data %p(%d)\n", c, c->next, c->back, c->data, *(int*)c->data);
        c = c->back;
    }
    list_pop(l);
    list_push(l, datai(2));
    c = l->tail;
    while (c) {
        //printf("self %p\tnext %p\t back %p\t data %p(%d)\n", c, c->next, c->back, c->data, *(int*)c->data);
        c = c->back;
    }
    list_delete(l);
}
void test_all() {
    DList *l; 
    l = list_new();
    list_add(l, datai(2));
    list_add(l, datas("sdf"));
    //printf("%d\n", *(int*)l->head->next->data);
    list_add(l, datas("str"));
    //printf("%s\n", (char*)l->head->data);
    list_print(l);
    list_p(l);
    list_delete(l);
    l = list_new();
    list_push(l, datai(1));
    list_push(l, datai(2));
    list_push(l, datai(2));
    list_p(l);
    list_delete(l);
    l = list_new();
    list_push(l, datai(1));
    list_push(l, datai(2));
    list_push(l, datai(3));
    list_add(l, datai(3));
    list_add(l, datai(3));
    /*
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
    */
    list_p(l);
    list_delete(l);
    l = list_new();
    list_push(l, datai(3));
    list_push(l, datai(3));
    list_push(l, datai(3));
    list_add(l, datai(3));
    list_add(l, datai(3));
    list_remove(l);
    //printf("pop = %d len(%d)\n", *(int*)list_pop(l), l->length);
    list_remove(l);
    list_p(l);
    list_delete(l);
}
void test_p_empty() {
    printf("* TEST empty\n");
    DList *l; 
    l = list_new();
    list_add(l, datai(2));
    list_delete(l);
    list_p(l); // Will read free'd mem
    printf("* empty END\n");
}
void test_leaks() {
    DList* l = list_new();
    int a = 1;
    int b = 2;
    list_push(l, datai(3));
    list_push(l, datai(3));
    list_push(l, datai(3));
    list_pop(l);
    list_pop(l);
    list_pop(l);
    list_add(l, datai(3));
    list_add(l, datai(3));
    list_add(l, datai(3));
    list_pop(l);
    list_pop(l);
    list_pop(l);
    list_delete(l);
    l = list_new();
    list_add(l, datai(3));
    list_add(l, datai(3));
    list_add(l, datai(3));
    printf("3 rem\n");
    list_remove(l);
    list_remove(l);
    list_remove(l);
    list_push(l, datai(3));
    list_push(l, datai(3));
    list_push(l, datai(3));
    list_remove(l);
    list_remove(l);
    list_remove(l);
    list_delete(l);
}
void test_save() {
    DList* l = list_new();
    list_push(l, datai(3));
    list_add(l, datai(3));
    list_save2file(l, "testsavelist");
    list_delete(l);
}

void test_load() {
    DList *l = list_loadfile("testsavelist");
    printf("loaded %d items\n", l->length);
    list_p(l);
    //list_map(l, list_free_data);
    list_delete(l);
}
void test_erase_at() {
    printf("* TEST ERASE AT\n");
    DList *l = list_new();
    list_push(l, datai(3));
    list_push(l, datai(3));
    list_push(l, datai(3));
    list_p(l);

    printf("* ERASE AT\n");
    list_erase_at(l, 1);
    list_p(l);
    list_push(l, datai(3));

    printf("* ERASE AT\n");
    list_erase_at(l, 2);
    list_p(l);
    list_push(l, datai(3));

    printf("* ERASE AT\n");
    list_erase_at(l, 0);
    list_p(l);

    list_delete(l);
}
void test_data() {
    printf("* TEST data\n");
    DList *l = list_new();

    printf("* only int\n");
    list_add(l, datai(1));
    list_add(l, datai(2));
    list_add(l, datai(8));
    list_print(l);
    list_add(l, datas("test string -----"));
    list_push(l, datas("test string2 -----"));
    list_erase_at(l, 3);
    list_add(l, datas("test string3 -----"));
    list_print(l);
    assert(l->length == 5);

    list_delete(l);
    printf("* data OK\n");
}

void test_dsvget() {
    printf("* TEST DSV\n");
    DList *l = list_new();
    list_push(l, datai(1));
    list_push(l, datai(4442));
    list_push(l, datai(8884442));
    list_add(l, datai(7773));
    list_add(l, datas("s7s773s"));
    list_push(l, datas("eNDs7s773s"));
    char *s = list_get_dsvstr(l);
    printf("DSV:---\n%s\n---\n", s);
    free(s);
    list_delete(l);

    l = list_from_dsvstr("DLST|1\nI32|81\nabracadabra");
    list_delete(l);

    printf("* OK DSV\n");
}

void test() {
    test_create();
    test_create_and_delete();
    test_create_and_remove();
    test_add();
    test_push();
    test_backs();
    test_pop();
    test_all();
    //test_p_empty();
    test_leaks();
    //test_save();
    //test_load();
    test_erase_at();
    test_data();
    test_dsvget();
    return;
}

int main() {
    test();
    return 0;
}

