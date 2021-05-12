#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main() {

    // Open a new socket
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if(udp_socket == -1) {
        printf("Open socket: failed\n");
        return 1;
    }

    printf("Open socket: OK\n");

    // Server config
    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    int bind_status = bind(udp_socket, (struct sockaddr *) &server, sizeof(server));

    if(bind_status == -1) {
        printf("Bind: failed");
        return 1;
    }

    printf("Bind: OK\n");


    // Get client message
    struct sockaddr_in client;
    unsigned char buffer[512];

    int client_size = sizeof(client);
    int recv_status = recvfrom(udp_socket, buffer, 512, 0, (struct sockaddr *) &client, &client_size);

    if (recv_status == -1) {
        printf("Get message: failed\n");
        return 1;
    }

    printf("Client message: %s\n", buffer);

    close(udp_socket);

    return 0;
}
