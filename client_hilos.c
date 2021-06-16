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

int sock_fd;

void *getMessages(void *data) {
    char cadena[100];
    while(!strstr(cadena, "bye")) {
        bzero(cadena, 100);
        read(sock_fd, cadena, 100);
        printf("Server: %s\n", cadena);
    }
}

void *sendMessages(void *data) {
    char sendline[100];
    while(!strstr(sendline, "bye")) {
        bzero(sendline, 100);
        gets(sendline);
        write(sock_fd, sendline, strlen(sendline) + 1);
    }
}

int main() {
    struct sockaddr_in servaddr;



    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5000);

    inet_pton(AF_INET, "10.0.2.15", &(servaddr.sin_addr));
    connect(sock_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    printf("\n\n------Inicio el chat EQUIPO3------\n\n");
    pthread_t p1;
    pthread_t p2;

    pthread_create(&p1, NULL, &getMessages, "get");
    pthread_create(&p2, NULL, &sendMessages, "send");

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

	printf("\n\nConversacion finalizada\n");
    return 0;
}