#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

// split the string by delimiter
char* strtok(char* str, const char* delim) {
    static char* ptr;
    if (str) {
        ptr = str;
    }
    if (*ptr == '\0') {
        return 0;
    }
    char* ret = ptr;
    while (*ptr != '\0' && *ptr != *delim) {
        ptr++;
    }
    if (*ptr == *delim) {
        *ptr = '\0';
        ptr++;
    }
    return ret;
}

int
main(int argc, char* argv[]) {

    // argument checking
    if (argc < 2) {
        fprintf(2, "xargs: no command given\n");
        exit(1);
    }
    
    char* cmd = argv[1];
    char* args[MAXARG];
    int i = 0;

    // copy the arguments to a new array
    // the last argument is the input: echo "hello" | xargs echo bye -> xargs echo bye "hello" 
    for(i = 1; i < argc; i++) {
        args[i-1] = argv[i];
    }

    // read from stdin to c
    char c;

    // buffer serves as a temporary storage for the input line
    char buf[512];
    // count number of characters in the buffer
    int cnt = 0;
    while (read(0, &c, sizeof(char)) != 0 ) {
        // parent
        // split the input by '\n'
        // create a child process for each line
        // child send to the parent argument via a pipe

        if (c == '\n') {
            buf[cnt] = '\0';
            if (fork() == 0) {
                // child
                // set the last argument to the read line
                // read from buffer, argument seperated by ' '
                char* token = strtok(buf, " ");
                while (token != 0) {
                    args[argc-1] = token;
                    token = strtok(0, " ");
                    argc++;
                }
                exec(cmd, args);
                exit(0);
            }
            else {
                // parent
                // wait for the child to finish
                wait(0);

                // reset the buffer
                cnt = 0;
                memset(buf, 0, sizeof(buf));
            }
        }
        else {
            // add c to buffer if c is not '\n'
            buf[cnt++] = c;
        }
    }
}