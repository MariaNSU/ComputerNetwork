#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#define MAX_BUFFER_SIZE 5
#define MAX_MESSAGE_SIZE 15

typedef struct chunk {
    char buf[MAX_BUFFER_SIZE]; 
    unsigned int ckSum;
    unsigned int number; 
    unsigned int quantity; 

} chunk;

void printChunk(chunk* ck){
    printf("chunk buffer: %s\n", ck->buf);
    printf("chunk ckSum: %d\n", ck->ckSum);
    printf("chunk number: %d\n", ck->number);
    printf("chunk quantity: %d\n", ck->quantity);
}

//func for calculating check sum
unsigned int checkSum(char* message) {
    unsigned int sum = 0;
    while(*message){
        sum+= *message;
        message ++;
    }
    return sum;
};

// divide message into chunks and returns massive of chunks
chunk* divideMessage(char* message){
    size_t len = strlen(message);
    unsigned int N = len / (MAX_BUFFER_SIZE) + 1;
    chunk* chunks = malloc(sizeof(chunk)*N);
    char buf[MAX_BUFFER_SIZE + 1]; 
    int ckSum = 0;
    unsigned int number; 
    for (int i = 0; i < N; i++) {
        int start = i*MAX_BUFFER_SIZE;
        int chunkSize = MAX_BUFFER_SIZE < len - start ? MAX_BUFFER_SIZE: len - start;
        memmove(chunks[i].buf, message + start, chunkSize);
        chunks[i].buf[chunkSize] = '\0'; 
        chunks[i].ckSum = checkSum(chunks[i].buf);
        chunks[i].number = i; 
        chunks[i].quantity = N;
    }
    return chunks;
}
