#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(){

    int cd;
    struct sockaddr_in addr;
    if((cd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("error calling socket");
        return __LINE__;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(2023);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(-1 == connect(cd, (struct sockaddr*)&addr, sizeof(addr))){
        perror("error connect");
        return __LINE__;
    }

    





    char name[255];
    scanf("%s", name);
    int nameLength = strlen(name);

    FILE* file = fopen(name, "rb");
    if(!file){
        perror("file has not found");
        return __LINE__;
    }
    int fileSize = 0;
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);
    printf("%d\n", fileSize);


    send(cd, &nameLength, 4, 0);
    send(cd, &fileSize, 4, 0);
    send(cd, &name, nameLength, 0);
    char data[1] = {0};
    while(fread(data, 1, 1, file) != 0){
        if(send(cd, data, sizeof(data), 0) == -1){
            perror("error send");
            fclose(file);
            close(cd);
            return __LINE__;
        }
    }


    fclose(file);
    close(cd);
    return 0;
}