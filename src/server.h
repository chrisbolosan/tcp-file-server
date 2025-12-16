#ifndef SERVER_H
#define SERVER_H

#include <string>
using namespace std;

//server functions
int initialize_server(const int port);
void send_message(int connection_sock, const string &msg);
void handle_request(const char* root, char* buffer, int connection_sock);
char* extract_request_path(char* request);
void sanitize_slashes(char* path);
bool is_directory(const char* path);
bool is_file(const char* path);
void show_directory(const char* root, int connection_sock);
char* build_full_path(const char* root, char* path);
void return_file_contents(const char* path, int connection_sock);
void chomp(char *s);

#endif
