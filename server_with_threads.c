#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define green "\x1b[32m"
#define reset "\x1b[0m"

int socket_descriptor, client_accepted;

void *getMessages(void* data) {
    char str[1024];
    while(!strstr(str, "bye")) {
        bzero(str, 1024);
        read(client_accepted, str, 1024);
        printf(green "cliente: %s", str);
        printf(reset "\n");
    }
}

void *sendMessages(void* data) {
    char sendline[1024];
    while(!strstr(sendline, "bye")) {
        bzero(sendline, 1024);
        gets(sendline);
        write(client_accepted, sendline, strlen(sendline)+1);
    }
}

int main() {
    struct sockaddr_in server;

    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htons(INADDR_ANY);
    server.sin_port = htons(5000);

    bind(socket_descriptor,(struct sockaddr *)&server, sizeof(server));
    listen(socket_descriptor, 10);
    client_accepted = accept(socket_descriptor, (struct sockaddr*)NULL, NULL);

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
