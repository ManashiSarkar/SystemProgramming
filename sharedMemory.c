#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define MEM_SZ 1024

int modifier(char * mem,int proc) {
    static char * s[2] = {"Child", "Parent"};
    static int cnt[2] = {0};
    printf("Inside %s, ",s[proc]);
    return strncpy(mem, s[proc], strlen(s[proc]) + 1), ++cnt[proc];
}

int main(void) {
    void *mem = mmap(NULL, MEM_SZ, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    if(mem == NULL) {
        printf("Failed to map memory: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /*
    pid_t pid = fork();
    if(pid == 0) {
        // Child process
        //sleep(1);
        while (1) {
            printf("%s has modified mem %d times\n", (char*) mem, modifier(mem,0));
            sleep(1);
        }
    } else {
        // Parent process
        //const char* msg = "Hello World!";
        //strncpy(mem, msg, strlen(msg) + 1);
        while (1) {
            printf("%s has modified mem %d times\n", (char*) mem, modifier(mem,1));
            sleep(1);
        }
    }

    munmap(mem, MEM_SZ);
    */
    pid_t pid = fork();
    char last_msg[2][100] = {0};
    strncpy(mem, "hello", strlen("hello") + 1);
    while (1) {
        if(pid == 0) {
            while (!strcmp(last_msg[0], mem))
                sleep(0.321);
            printf("%s has modified mem %d times\n", (char*) mem, modifier(mem,0));
        } else {
            while (!strcmp(last_msg[1], mem))
                sleep(0.788);
            printf("%s has modified mem %d times\n", (char*) mem, modifier(mem,1));
        }
        //sleep(0.87);
    }

    munmap(mem, MEM_SZ);

    return 0;
}
