#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syscall.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


struct Buffer {
    char* buf;
    size_t size;
};

size_t BUFSIZE = 2;

struct Buffer readMessage(int fd){
    size_t wasRead = 0, total = 0, freeSpace = BUFSIZE*4;
    int inc = 2;

    char* buf = (char*)malloc(BUFSIZE*sizeof(char));
    if(buf == NULL){
        perror("memory allocation for buf failed!");
        exit(1);
    }

    struct Buffer B;
    B.buf = (char*)malloc(freeSpace*sizeof(char));
    if(B.buf == NULL){
        perror("memory allocation for B.buf failed!");
        exit(1);
    }
  for(int i = 1; ;i++){
        wasRead = read(fd, buf, BUFSIZE);
        freeSpace -= wasRead;
        if(freeSpace <= 0){
            freeSpace = BUFSIZE*4;
            B.buf = (char*)realloc(B.buf,freeSpace*(inc));
            if(B.buf == NULL){
                perror("memory reallocation for B.buf failed!");
                exit(1);
            }
            inc+=1;
        }
        memcpy(B.buf + total, buf, wasRead);
        total+= wasRead;
        if (wasRead == -1 ){
            perror("read failed!");
            exit(1);
        }
        if (wasRead < BUFSIZE){
            break;
        }  
    }
    B.size = total;
    return B;
}


int main(){
    
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0){
        perror("Can not create a server socket!");
        return 0;
    }
    struct sockaddr_in servAddr;
    memset((char*)&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(1040);
    inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.s_addr);

    if (bind(s, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("server bind failed");
        return 0;
    }
    if (listen(s, 2) < 0) {
		perror("listen failed");
		return 0;
	}
    int rqst;
    struct sockaddr_in cliAddr;
    socklen_t len;
    struct Buffer B;
    
	while(1) {
		while ((rqst = accept(s, (struct sockaddr *)&cliAddr, &len)) > 0) {
            B = readMessage(rqst);
            char* invertedBuf = malloc(B.size * sizeof(char));  
            for(int i = B.size - 1; i >= 0 ; i--){
                int j = 0;
                invertedBuf[j] = B.buf[i];
                j++;
            }
            printf("%s\n", invertedBuf);
            int writen = write(rqst, invertedBuf, sizeof(invertedBuf));
            if(writen == -1){
                perror("wtite failed!");
            }
        }
        if (rqst == -1){
            perror("accept failed!");
        }
	}
    
    return 0;
}
