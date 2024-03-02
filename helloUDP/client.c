#include "utility.h"
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1000000
#define MAX_NUMBER_PACK 10

int main() {
    int sockfd;
    struct sockaddr_in destAddr;
    char buffer [MAX_BUFFER_SIZE];
    int sending;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return errno;
    }

    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(1040);
    inet_pton(AF_INET, "127.0.0.1", &destAddr.sin_addr.s_addr);


    for(int i = 0; i < MAX_NUMBER_PACK; i++){
        printf("Enter a message to send: ");
        scanf("%s", buffer);
        char* newBuf = (char*)malloc(sizeof(char)*strlen(buffer) + sizeof(double)*2);
        if(newBuf == NULL){
            printf("Malloc failed!\n");
            break;
        }
        newBuf = sendingMessage(buffer, i);
        sending = sendto(sockfd, newBuf, strlen(newBuf), 0, (const struct sockaddr *) &destAddr, sizeof(destAddr));
        if(sending < 0){
            perror("Sendto failed!");
        }
        printf("You send: %s\n", newBuf);
        
    }

    return 0;
}
