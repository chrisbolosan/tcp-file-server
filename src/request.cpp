#include "server.h"
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <netinet/in.h> 
#include <arpa/inet.h>   
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

// Extract path from GET request
char* extract_request_path(char* request) {
    if (strncmp(request, "GET ", 4) != 0) return NULL;
    char* path = request + 4;
    if (*path != '/') return NULL;

    char* first_space = strchr(path, ' ');
    if (first_space != NULL) {
        char* p = first_space + 1;
        while (*p != '\0') { if (*p != ' ') return NULL; p++; }
        *first_space = '\0';
    }

    if (strchr(path, ' ') != NULL) return NULL;
    return path;
}

// Build full path root + request
char* build_full_path(const char* root, char* path) {
    char* full = (char*) malloc(1024);
    if (!full) return NULL;
    if (path[0] == '/') snprintf(full, 1024, "%s%s", root, path);
    else snprintf(full, 1024, "%s/%s", root, path);
    return full;
}

// Send file contents
void return_file_contents(const char* path, int connection_sock) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) { write(connection_sock, "Error opening file\n", 19); return; }

    char* buffer = (char*) malloc(1024);
    int bytes_read;
    while ((bytes_read = read(fd, buffer, 1024)) > 0)
        write(connection_sock, buffer, bytes_read);

    free(buffer);
    close(fd);
}

// Show directory contents
void show_directory(const char* root, int connection_sock) {
    DIR* dirp = opendir(root);
    if (!dirp) { write(connection_sock, "Can't open directory\n", 20); return; }

    struct dirent* entry;
    char* line = (char*) malloc(512);
    if (!line) { closedir(dirp); return; }

    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        snprintf(line, 512, "%s ", entry->d_name);
        write(connection_sock, line, strlen(line));
    }

    free(line);
    closedir(dirp);
}

