#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    // argument handling
    if (argc != 1) {
        fprintf(2, "pingpong: no arguments needed\n");
        exit(1);
    }

    int fds_p[2]; // file descriptors
    pipe(fds_p); // create pipe from parent to child

    int fds_c[2];
    pipe(fds_c); // create pipe from child to parent

    int pid = fork();   

    if (pid > 0) {
        // parent
        int pid_parent = getpid();
        
        char parent_msg = 's';
        write(fds_p[1], &parent_msg, 1); 

        close(fds_p[1]);
        wait(0);

        if(read(fds_c[0], &parent_msg, 1)){
            printf("%d: received pong\n", pid_parent);
        }

        close(fds_c[0]);
        exit(0);
    }
    else if (pid == 0) {
        // child
        int pid_child = getpid();

        char child_msg;
        if(read(fds_p[0], &child_msg, 1)){
            printf("%d: received ping\n", pid_child);
        }
        close(fds_p[0]); 

        write(fds_c[1], &child_msg, 1); // write 1 byte to the parent;
        
        close(fds_c[1]); // close the write end of the pipe;
        exit(0); // exit the child;
    } else {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    exit(0);
}
// Tan's code