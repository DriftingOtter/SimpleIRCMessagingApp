#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


int createSocket();
struct sockaddr_in socketConfig(); 


int main(int argc, char** argv){

    // TODO
    // Create Arg Parser That Checks For IRC Server Creation OR Joining Server

    return 0;
}

int createSocket(){
    // Creating a client socket via socket() function call
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Check if socket creation failed
    if (clientSocket == -1) {
        perror("Socket creation failed");
        return -1;
    }

    return clientSocket;
}

struct sockaddr_in socketConfig() {
    // Creating struct that holds server socket template
    struct sockaddr_in server_addr;

    // Setting server settings/configs
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9001); // Using the same port as the server
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (server_addr.sin_family != AF_INET) {
        perror("Socket [sin_family] not correctly allocated.\n");
        return server_addr;
    }
    if (server_addr.sin_port != htons(9001)) {
        perror("Socket [sin_port] not correctly allocated.\n");
        return server_addr;
    }
    if (server_addr.sin_addr.s_addr != INADDR_ANY) {
        perror("Socket [sin_addr.s_addr] not correctly allocated.\n");
        return server_addr;
    }

    return server_addr;
}

