#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syscall.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

int main(){
    struct sockaddr_in servAddr, senderAddr, senderAddr2;
    socklen_t len = sizeof(senderAddr);
    int count = 0, mesNum = 0;
    unsigned int recCheckSum = 0;    
    int recData;
    
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s < 0){
        perror("Can not create a server socket!");
        return 0;
    }
   
    memset((char*)&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(1040);
    inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.s_addr);

    if (bind(s, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("server bind failed");
        return 0;
    }
    chunk recvChunk;

	while(1) {
		recData = recvfrom(s, &recvChunk, sizeof(recvChunk), 0, (struct sockaddr *) &senderAddr, &len);
        
        if (recData <= 0){
            perror("Message was not received!");
            continue;
        }
        if(senderAddr2.sin_addr.s_addr != senderAddr.sin_addr.s_addr){
            count = 0;
        }
        else{
            count ++;
        }
        if(checkSum(recvChunk.buf) != recvChunk.ckSum){
            printf("Message was diverged!\n");
            continue;
        }
        if(count != recvChunk.number){
            printf("Message number %d was lost!\n", count);
            continue;
        }
        senderAddr2 = senderAddr;
        printChunk(&recvChunk);
	}
    return 0;
}
