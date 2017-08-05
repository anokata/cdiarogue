#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>

char* popen_read(char* cmd) {
    FILE* fd = popen(cmd, "r");
    if (fd == 0) {
        perror("popen");
        exit(2);
    }
    //int c = 1;
    static char buf[100];
    //while (c != EOF) {
    fread(buf, 99, 1, fd);
    //}
    int s = pclose(fd);
    if (s != 0) {
        perror("pclose");
        exit(2);
    }
    return buf;
}
//shmget, shmat, shmdt, shmctl
// ftok
// cmd: ipcs
int main(int argc, char** argv) { // Create 2 segments // exec cmd 
    //if (argc < 2) return 0;
    key_t key, key2;
    if ((key = ftok("sh_mem.c", 88)) < 0) {
        perror("ftok");
        return 1;
    }
    if ((key2 = ftok("sh_mem.c", 89)) < 0) {
        perror("ftok");
        return 1;
    }
    int id = shmget(key, 1000, 0666 | IPC_CREAT);
    if (id < 0) {
        perror("shmget");
        return 1;
    }
    int id2 = shmget(key2, 1000, 0666 | IPC_CREAT);
    if (id2 < 0) {
        perror("shmget");
        return 1;
    }
    printf("id2=%d\n", id2);
    int* addr = shmat(id, NULL, 0);
    int* addr2 = shmat(id2, NULL, 0);
    if ((addr < 0) || (addr2 < 0)) {
        perror("shmat");
        return 1;
    }
    printf("addr=%p\n", addr);
    // write nums
    for (int i=0; i < 100; i++) {
        addr[i] = i+10;
        addr2[i] = i*2;
    }
    puts("writed\n");
    // exec. 
    char cmd[100];
    sprintf(cmd, "./sh_mem_sum %d %d", key, key2);
    puts(cmd);
    char* buf = (popen_read(cmd));
    key_t newkey = atoi(buf);
    printf("buf= %s %d\n", buf, newkey);

    int id3 = shmget(newkey, 1000, 0666);
    int* adr3 = shmat(id3, NULL, 0);
    for (int i=0; i < 10; i++) {
        printf("%d ", adr3[i]);
    }
    shmdt(adr3);

    
    if ((shmdt(addr) < 0) || (shmdt(addr2) < 0)) {
        perror("shmdt");
        return 1;
    }
    printf("key1=%d key2=%d\n", key, key2);
    return 0;
}
