#include "kernel/types.h"
#include "user/user.h"

void sieve(int left_p[2]) __attribute__((noreturn));
void sieve(int left_p[2]) {
    close(left_p[1]);  // close write end of left pipe
    
    int p;
    // check if this is the right most child: no data to read, close read end
    if (read(left_p[0], &p, sizeof(int)) == 0) {
        close(left_p[0]);  // no data to read, close read end
        exit(0);
    }

    printf("prime %d\n", p);

    // pipe feed to the right child of the current process
    int right_p[2];
    pipe(right_p);

    if (fork() > 0) {
        close(right_p[0]);  // parent only writes to right pipe
        
        int n;
        while (read(left_p[0], &n, sizeof(int)) > 0) {
            if (n % p != 0) { // if n is not divisible by p, write to right pipe, this serves like a filter
                write(right_p[1], &n, sizeof(int));
            }
        }
        close(right_p[1]);  // close write end
        close(left_p[0]);   // close read end
        wait(0);            // wait for child to finish
        exit(0);
    } else {
        close(left_p[0]);  // close read of left pipe before continuing
        sieve(right_p);    // recursive call for child process
    }
}


int
main(int argc, char* argv[]) {

    // argument checking
    if (argc != 1) {
        fprintf(2, "Usage: primes\n");
        exit(1);
    }

    // create a pipe
    int p[2];
    pipe(p);

    if (fork() > 0) {
         // parent
        // feed the numbers from 2 to 280 to the pipe for the child to sieve
        for (int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);  // close write end
        wait(0);      // wait for child to finish
    } else {
        // child    
        sieve(p);
    }
    exit(0);
}
