#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;
    struct ip_mreq group;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    group.imr_multiaddr.s_addr = inet_addr("224.0.0.1");
    group.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &group, sizeof(group)) < 0) {
        perror("Adding multicast group error");
        exit(EXIT_FAILURE);
    }

    int n, len;

    while (1) {
        n = recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL, NULL, NULL);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);
    }

    close(sockfd);

    return 0;
}

