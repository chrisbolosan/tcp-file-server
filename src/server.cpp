/*
 * Simple TCP File Server
 * Author: Chris Bolosan, 2025
 *
 * Licensed under the MIT License. See LICENSE file for details.
 *
 * This code is intended for educational and personal use.
 * Copying this code for assignments or other academic submissions
 * without permission is prohibited and may violate academic honesty policies.
 */

#include "server.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <cstdio>

using namespace std;

int initialize_server(const int port) {
    int server;
    struct sockaddr_in server_addr;
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Failure to create server socket"); exit(EXIT_FAILURE); }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    if (::bind(server, (struct sockaddr*)&server_addr, (socklen_t)sizeof(server_addr)) < 0)
	{
        	perror("Error binding"); 
		exit(EXIT_FAILURE); 
	}

    if (listen(server, 5) < 0) { perror("Failed to listen"); exit(EXIT_FAILURE); }

    cout << "TCP Server listening on port " << port << " ..." << endl;
    return server;
}

void send_message(int connection_sock, const string &msg) 
{
    write(connection_sock, msg.c_str(), msg.length());
}

void handle_request(const char* root, char* buffer, int connection_sock)
{
	char* path = extract_request_path(buffer);

	if (!path) { send_message(connection_sock, "Not a valid request\n"); return; }

	sanitize_slashes(path);

	if (strstr(path, "..")) { send_message(connection_sock, "Not a valid request\n"); return; }

	char* full_path = build_full_path(root, path);

	if (!full_path) return;

	if (is_directory(full_path)) 
	{
		char index_path[1024];
		snprintf(index_path, sizeof(index_path), "%s/index.html", full_path);

		if (is_file(index_path)) { return_file_contents(index_path, connection_sock); }
	else 
	{
		show_directory(full_path, connection_sock); send_message(connection_sock, "\n");
	}

	free(full_path); return;
	}

	if (is_file(full_path)) 
	{ 
		return_file_contents(full_path, connection_sock); free(full_path); return;
	}
	else
	{

		path++;
		char* error_message = (char*) malloc(128);
		snprintf(error_message, 128, "Error: %s not found\n", path);
		send_message(connection_sock, error_message);
		free(error_message);
	}

	free(full_path);
}

