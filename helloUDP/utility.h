#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//func for calculating check sum
double checkSum(char* message) {
    double base = 64.0;
    double sum = 0;

    size_t len = strlen(message);
    if(len == 0){
        printf("Empty message!\n");
        return 0;
    }
    for(int i = 0; i < len; i++){
        sum+= message[i]*pow(base, len - i - 1);
    }
    return sum;
}
//func for creating message with check sum, number of message and message text 
char* sendingMessage(char* message, int number){
    double sum = checkSum(message);
    size_t len = strlen(message);
    char* result = (char*)malloc(sizeof(char)*len + sizeof(double)*2);
    sprintf(result, "%lf %d %s", sum, number + 1, message);
    return result;

}