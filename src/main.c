#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int createSocket();
struct sockaddr_in socketConfig(int avalibleSocket);
int findFreePort();


int main(int argc, char** argv){

    int usrChoice = 0;

    do{
        printf("-----IRC Chat Application-----\n");
        printf("|  [1] - Create New Chat Room \n");
        printf("|  [2] - Join Chat Room       \n");
        printf("|  [3] - Quit                 \n");
        printf("------------------------------\n");
        printf("\n\n");

        printf("Enter [1-3]: ");

        int input = scanf("%i", &usrChoice);
        if (input == -1){
            perror("Error upon inputting user value.\n");
            exit(EXIT_FAILURE);
        }
    } while (usrChoice == 0);


    if (usrChoice == 1){

        int avaliblePort = findFreePort();
        if (avaliblePort == -1){
            perror("Error When Creating New Room: No Bindable Ports On Network.\n");
            exit(EXIT_FAILURE);
        }

        // Create Usr Socket and Server Config
        struct sockaddr_in server_addr = socketConfig(avaliblePort);
        int usrSocket = createSocket();

        // Binding to network port
        if (bind(usrSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
            perror("Bind failed");
            exit(EXIT_FAILURE);
        } else {
            printf("Sucessfully created room [%i]\n", avaliblePort);
        }

        // Listening for connections...
        if (listen(usrSocket, 1) == -1) {
            perror("Listen failed");
            exit(EXIT_FAILURE);
        } else {
            printf("Room: Waiting For Users...\n");
        }
    }

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

struct sockaddr_in socketConfig(int avalibleSocket) {

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(avalibleSocket); // Bind to any available network interface
    server_addr.sin_port = 0; // Let the OS assign a random port
    memset(&(server_addr.sin_zero), '\0', 8);

    if (server_addr.sin_family != AF_INET) {
        perror("Socket [sin_family] not correctly allocated.\n");
        return server_addr;
    }
    if (server_addr.sin_port != htons(avalibleSocket)) {
        perror("Socket [sin_port] not correctly allocated.\n");
        return server_addr;
    }
    if (server_addr.sin_addr.s_addr != INADDR_ANY) {
        perror("Socket [sin_addr.s_addr] not correctly allocated.\n");
        return server_addr;
    }

    return server_addr;
}


int findFreePort() {
    int avaSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (avaSocket < 0) {
        perror("Socket creation failed");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to any available network interface
    server_addr.sin_port = 0; // Let the OS assign a random port
    memset(&(server_addr.sin_zero), '\0', 8);

    // Bind the socket to a random port
    if (bind(avaSocket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
        perror("Socket binding failed");
        close(avaSocket);
        return -1;
    }

    // Retrieve the allocated port
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if (getsockname(avaSocket, (struct sockaddr *)&addr, &addr_len) == -1) {
        perror("getsockname() failed");
        close(avaSocket);
        return -1;
    }

    int freePort = ntohs(addr.sin_port);
    close(avaSocket);

    return freePort;
}
