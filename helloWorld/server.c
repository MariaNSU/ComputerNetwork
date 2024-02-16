#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syscall.h>

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
    char buf [256] = "Hello, world!\n";
	for (;;) {
		while ((rqst = accept(s, (struct sockaddr *)&cliAddr, &len)) > 0) {
            int writen = write(rqst, buf, sizeof(buf));
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