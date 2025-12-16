#include "server.h"
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h> 
#include <arpa/inet.h>   

// Removes trailing newline and carriage returns
void chomp(char *s) {
    int len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}

// Returns true if path is directory
bool is_directory(const char* path) {
    DIR* dir = opendir(path);
    if (dir) { closedir(dir); return true; }
    return false;
}

// Returns true if path is file
bool is_file(const char* path) {
    int fd = open(path, O_RDONLY);
    if (fd >= 0) { close(fd); return true; }
    return false;
}

// Remove duplicate slashes
void sanitize_slashes(char* path) {
    char* current = path;
    char* next = path;
    bool prev_slash = false;
    while (*current) {
        if (*current == '/') {
            if (!prev_slash) { *next++ = '/'; prev_slash = true; }
        } else { *next++ = *current; prev_slash = false; }
        current++;
    }
    *next = '\0';
}

