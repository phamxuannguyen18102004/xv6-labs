#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    // printf("p: %s\n", p);
    return p; // return file name
}

void find(char* path, char* file_name) {
    int fd;
    char buf[512], *p;
    struct dirent de;
    struct stat st;
    
    
    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if((fstat(fd, &st)) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    // if the path is a file, check if the filename is the same as the file_name
    // if the path is a directory, find all file the directory with the name file_name and print
    // check the subdirectories and find all file within it recursively     

    switch (st.type) {
        case T_FILE:
            if (strcmp(fmtname(path), file_name) == 0) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) {
                    continue;
                }

                // add folder name to p end
                strcpy(p, de.name);

                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                    continue;
                }
                
                find(buf, file_name);
            }
            break;
        
    }
    close(fd);
}

int
main(int argc, char *argv[]) {
    // argument checking
    if (argc != 3) {
        fprintf(2, "Usage: find <path> <file_name>\n");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);

}