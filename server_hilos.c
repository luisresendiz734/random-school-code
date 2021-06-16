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

int listen_fd, comm_fd;

void *getMessages(void* data) {
    char cadena[100];
    while(!strstr(cadena, "bye")) {
        bzero(cadena, 100);
        read(comm_fd, cadena, 100);
        printf("client: %s\n", cadena);
    }
}

void *sendMessages(void* data) {
    char sendline[100];
    while(!strstr(sendline, "bye")) {
        bzero(sendline, 100);
        gets(sendline);
        write(comm_fd, sendline, strlen(sendline)+1);
    }
}

int main() {
    struct sockaddr_in servaddr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(5000);

    bind(listen_fd,(struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listen_fd, 10);
    comm_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL);

    printf("\n\nConversacion iniciada\n\n");

    pthread_t p1;
    pthread_t p2;

    pthread_create(&p1, NULL, &getMessages, "get");
    pthread_create(&p2, NULL, &sendMessages, "send");

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    
	printf("\n\nConversacion finalizada\n");
    return 0;
}