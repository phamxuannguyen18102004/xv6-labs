#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char *argv[]) {

    // argument checking
    if (argc == 1) {
        // 0 stdin, 1 stdout, 2 stderr
        fprintf(2, "User forget to pass an argument.\n");
        exit(1);
    }

    if (argc > 2) {
        fprintf(2, "User pass too many arguments.\n");
        exit(1);
    }

    int ticks = atoi(argv[1]);
    if (ticks < 0) ticks = 0;

    // argv[1] is string of time
    
    fprintf(1, "(nothing happens for a little while)\n");
    sleep(ticks);    // sleep for some ticks
    exit(0);
}