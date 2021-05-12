#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
    
    // Your message to send
    unsigned char buffer[512] = "como vamos gerry :v";

    // Open a new socket
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if(udp_socket == -1) {
        printf("Open socket: failed\n");
        return 1;
    }

    printf("Open socket: OK\n");

    // Client config
    struct sockaddr_in client;

    client.sin_family = AF_INET;
    client.sin_port = htons(8080);
    client.sin_addr.s_addr = inet_addr("10.0.2.15");

    // Send message
    int sendto_status = sendto(udp_socket, buffer, strlen(buffer) + 1, 0, (struct sockaddr *) &client, sizeof(client));

    if (sendto_status == -1) {
        printf("Send message: failed\n");
        return 1;
    }

    printf("Send message: OK\n");

    close(udp_socket);

    return 0;
}
