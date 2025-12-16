#include "server.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h> 
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) { cerr << "Usage: " << argv[0] << " <port> <root-path>\n"; exit(EXIT_FAILURE); }

    const int port = atoi(argv[1]);
    char* root = argv[2];
    int server_sock = initialize_server(port);

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t clientlen = sizeof(client_addr);
        int connection_sock = accept(server_sock, (struct sockaddr*)&client_addr, &clientlen);
        if (connection_sock < 0) { perror("accept"); continue; }

        pid_t pid = fork();
        if (pid < 0) { perror("fork"); close(connection_sock); continue; }

        if (pid == 0) { // child process handles client
            close(server_sock);
            char* buffer = (char*) malloc(1024);
            if (!buffer) { perror("Memory allocation"); exit(EXIT_FAILURE); }

            int received = read(connection_sock, buffer, 1024 - 1);
            if (received < 0) { perror("read"); close(connection_sock); exit(EXIT_FAILURE); }

            buffer[received] = '\0';
            chomp(buffer);
            cout << "Client requested: " << buffer << endl;

            handle_request(root, buffer, connection_sock);
            send_message(connection_sock, "Connection closed by foreign host.\n");

            free(buffer);
            close(connection_sock);
            exit(EXIT_SUCCESS);
        } else { // parent
            close(connection_sock);
            waitpid(pid, NULL, 0);
        }
    }

    close(server_sock);
    return 0;
}

