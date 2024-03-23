#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syscall.h>
#include <unistd.h>
#include <stdbool.h>

#include "utility.h"

int main(){
    struct sockaddr_in servAddr, senderAddr;
    socklen_t len = sizeof(senderAddr);
    
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if(s < 0){
        perror("Can not create a server socket!");
        return -1;
    }
   
    memset((char*)&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(1040);
    inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.s_addr);

    if (bind(s, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("server bind failed");
        return -1;
    }
    //prepare for receive chunks from client 
    uint8_t mesNum = 0;
    uint8_t quantity = 0;
    uint8_t recCheckSum = 0;    
    int recData;
    uint8_t N = MAX_MESSAGE_SIZE/MAX_BUFFER_SIZE + 1;
    chunk recvChunk;
    chunk * chunksMassive = (chunk*)malloc(sizeof(chunk)*N);
    memset((chunk*)chunksMassive, 0, sizeof(*chunksMassive));
    bool checking [N];

    //prepare for using select() for waiting 
    int retval;
    fd_set rfds;
    struct timeval  tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0;

    
	while(1) {
        FD_ZERO(&rfds);
        FD_SET(s, &rfds);
        retval = select(s + 1, &rfds, NULL, NULL, &tv);

        if (retval == -1){
            perror("Select error!");
            continue;
        }
        //checking how many chunks were received
        if (retval <= 0) {
            printf("No data within 30 seconds\nChunks checking starts\n");
            if(quantity == 0){
                printf("Nothing to check!\n");
                break;
            }
            char mes[MAX_MESSAGE_SIZE];
            for(int i = 0; i < quantity; i++){
                if(!checking[i]) {
                    printf("Chunk %d was not received!\n", i);
                    sprintf(mes, "%s %u\n", "Not received: resend chunk number ", i);
                    int sending = sendto(s,mes, sizeof(mes),
                                0, (const struct sockaddr *) &senderAddr, sizeof(senderAddr));
                    if(sending < 0){
                        perror("Sendto failed!");
                    }
                }
            }
            printf("Checking ends! All chunks were succesfully received!\n"); 
            break;           
        }
        if (!FD_ISSET(s, &rfds)){
            printf("Server is not ready for reading!\n");
            continue;
        }
		recData = recvfrom(s, &recvChunk, sizeof(recvChunk), MSG_WAITALL, (struct sockaddr *) &senderAddr, &len);
        
        if (recData <= 0){
            perror("Message was not received!");
            continue;
        }
        
        //checking received data
        quantity = recvChunk.quantity;
        mesNum = recvChunk.number;

        if(mesNum >= N) {
            printf("Too large number of chunk\n");
            return -1;
        }

        chunksMassive[mesNum] = recvChunk;
        if(checkSum(recvChunk.buf) != recvChunk.ckSum){
            printf("Message was diverged!\n");
            char mes[MAX_BUFFER_SIZE];
            sprintf(mes, "%s %u\n", "Wrong checkSum: resend chunk number ", mesNum);
            int sending = sendto(s,mes, sizeof(mes),
                            0, (const struct sockaddr *) &senderAddr, sizeof(senderAddr));
            if(sending < 0){
                perror("Sendto failed!");
            }
            checking[mesNum] = 0;
            continue;
        }
        checking[mesNum] = 1;
	}
    if(quantity == 0){
        return 0;
    }
    printf("Server ended!\nReceived data:\n");
    for (int i = 0; i < quantity; i++){
        printf("[%d]:%s\n", i, chunksMassive[i].buf);
    }
    return 0;
}
