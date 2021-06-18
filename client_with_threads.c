#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define yellow "\x1b[33m"
#define reset "\x1b[0m"

int socket_descriptor;

void *getMessages(void *data) {
    char str[1024];
    while(!strstr(str, "bye")) {
        bzero(str, 1024);
        read(socket_descriptor, str, 1024);
        printf(yellow "servidor: %s", str);
        printf(reset "\n");
    }
}

void *sendMessages(void *data) {
    char sendline[1024];
    while(!strstr(sendline, "bye")) {
        bzero(sendline, 1024);
        gets(sendline);
        write(socket_descriptor, sendline, strlen(sendline) + 1);
    }
}

int main() {
    struct sockaddr_in client;

    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&client, sizeof(client));

    client.sin_family = AF_INET;
    client.sin_port = htons(5000);

    inet_pton(AF_INET, "10.0.2.15", &(client.sin_addr));
    connect(socket_descriptor, (struct sockaddr*)&client, sizeof(client));

    printf("\n\n--- Chat Equipo 3 ---\n\n");
    pthread_t p1;
    pthread_t p2;

    pthread_create(&p1, NULL, &getMessages, "get");
    pthread_create(&p2, NULL, &sendMessages, "send");

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

	printf("\n\nConversacion finalizada\n");
    return 0;
}
