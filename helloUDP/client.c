#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1000000

int main() {
    int sockfd;
    struct sockaddr_in destAddr;
    char buffer[MAX_BUFFER_SIZE];
    int sending;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return errno;
    }

    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(1040);
    inet_pton(AF_INET, "127.0.0.1", &destAddr.sin_addr.s_addr);


    printf("Enter a string to send: ");
    scanf("%s", buffer);
    sending = sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &destAddr, sizeof(destAddr));
    if(sending < 0){
        perror("sendto failed!");
    }

    return 0;
}
