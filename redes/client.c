#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SEVPORT 3333
#define MAXDATASIZE 1024

int main(int argc, char *argv[]) {
    int sockfd, sendbytes;
    char buf[MAXDATASIZE];
    struct hostent *host;
    struct sockaddr_in serv_addr;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <hostname> <filename>\n", argv[0]);
        exit(1);
    }

    if ((host = gethostbyname(argv[1])) == NULL) {
        perror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SEVPORT);
    serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(serv_addr.sin_zero), 8);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

    // Check if file exists in the 'arquivos' directory
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "arquivos/%s", argv[2]);

    if (access(filepath, F_OK) != -1) {
        snprintf(buf, sizeof(buf), "arquivo %s encontrado", argv[2]);
    } else {
        snprintf(buf, sizeof(buf), "erro arquivo %s nao encontrado", argv[2]);
    }

    if ((sendbytes = send(sockfd, buf, strlen(buf), 0)) == -1) {
        perror("send");
        exit(1);
    }

    printf("Message sent: %s\n", buf);

    close(sockfd);
    return 0;
}
