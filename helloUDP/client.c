#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utility.h"

int main() {
    int sockfd;
    struct sockaddr_in destAddr;
    char message [MAX_MESSAGE_SIZE];
    int sending;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(1040);
    inet_pton(AF_INET, "127.0.0.1", &destAddr.sin_addr.s_addr);
    
    printf("Enter a message to send: \n");
    fgets(message, MAX_MESSAGE_SIZE, stdin);

    chunk* dividedMes = divideMessage(message);
    unsigned int N = dividedMes->quantity;

    for(int i = 0; i < N; i++){

        sending = sendto(sockfd, &dividedMes[i], sizeof(dividedMes[i]), 
                        0, (const struct sockaddr *) &destAddr, sizeof(destAddr));
        if(sending < 0){
            perror("Sendto failed!");
            continue;
        }
        printf("[%d]\n", i);
        printChunk(&dividedMes[i]);
    
    }

    return 0;
}
