#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syscall.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 1000000

int main(){
    struct sockaddr_in servAddr, senderAddr;
    socklen_t len = sizeof(senderAddr);
    char buffer [MAX_BUFFER_SIZE];
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

	while(1) {
		recData = recvfrom(s, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *) &senderAddr, &len);
        if (recData > 0){
            buffer[recData] = '\0';
            printf("Received message: %s\n", buffer);
        }
        
	}
    
    return 0;
}