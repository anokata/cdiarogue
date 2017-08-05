#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc < 2) return 0;
    key_t s1, s2;
    s1 = atoi(argv[1]);
    s2 = atoi(argv[2]);
    //printf("skey %d\n", s1);
    //printf("skey %d\n", s2);
    key_t key;
    if ((key = ftok("Makefile", 18)) < 0) {
        perror("ftok");
        return 1;
    }
    //printf("key %d\n", key);
    int id = shmget(key, 1000, 0666 | IPC_CREAT);
    if (id < 0) {
        perror("shmget");
        return 1;
    }
    //printf("new id %d\n", id);
    int id2 = shmget(s1, 1000, 0666);
    int id3 = shmget(s2, 1000, 0666);
    if ((id2 < 0) || (id3 < 0)) {
        perror("shmget");
        return 1;
    }
    int* addr = shmat(id, NULL, 0);
    int* addr2 = shmat(id2, NULL, 0);
    int* addr3 = shmat(id3, NULL, 0);
    if ((addr < 0) || (addr2 < 0)) {
        perror("shmat");
        return 1;
    }
    int i;
    for (i=0; i < 100; i++) {
        addr[i] = addr2[i] + addr3[i];
    }
    //puts("sumed\n");
    
    if ((shmdt(addr) < 0) || (shmdt(addr2) < 0) || (shmdt(addr3) < 0)) {
        perror("shmdt");
        return 1;
    }
    printf("%d\n", key);
    return 0;
}
