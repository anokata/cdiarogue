#include <stdio.h>
#include <stdint.h>

typedef enum ekey_type {KEY_UINT32, KEY_STR} ekey_type;

typedef union key_u {
    uint32_t ukey;
    char *skey;
    double dkey;
} key_u;

struct some {
    int b;
    int a;
};

struct some fs() {
    struct some s;
    s.b = 1;
    s.a=2;
    return s;
}

int main() {
    union key_u k;
    key_u e;
    ekey_type t;
    t = KEY_UINT32;
    k.dkey = 0.11110202001;
    k.ukey = 2002;
    k.skey = "string";
    e.dkey = 0.123;
    printf("size %ld bytes\t u:%d| f:%f| s:%s| e.d:%f| \n", sizeof(k), k.ukey, k.dkey, k.skey, e.dkey);
    printf("size enum %ld\t %d %d %d\n", sizeof(ekey_type), t, KEY_UINT32, KEY_STR);
    struct some aaa = fs();
    aaa.a = 8;
    printf("struct auto %d %d\n", aaa.a, aaa.b);
    printf("struct auto %d %d\n", fs().a, fs().b);
    return 0;
}
