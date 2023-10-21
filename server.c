#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
	int sd, cd;
	struct sockaddr_in addr;
	int NameLenght = 0;
	int FileSize = 0;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		perror("Error calling socket");
		return __LINE__;
	
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(2023);
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");


	if(bind(sd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
		perror("bind");
		return __LINE__;
	}

	if(listen(sd, 1)){
		perror("listen");
		return __LINE__;
	}
	printf("Waiting for connection...\n");










	while((cd = accept(sd, NULL, NULL)) > 0){
        printf("Connected\n");
        
		//Иногда recv считывает только 1 байт, из-за чего размер определяется неправильно
		int* NameSize = (int*)malloc(sizeof(int)*2);
		int a = recv(cd, NameSize, 8, 0);
		printf("%d bytes recieved for name length and file size\n", a);
		NameLenght = NameSize[0];
		FileSize = NameSize[1];
		free(NameSize);
		
		printf("Name length: %d\tFile size: %d\n", NameLenght, FileSize);
		if(NameLenght < -255 || NameLenght > 255 || NameLenght == 0){
			printf("Incorrect name length\n");
			close(cd);
			close(sd);
			return __LINE__;
		}


		char* name = (char*)malloc(sizeof(char)*NameLenght);
		recv(cd, name, NameLenght, 0);

		if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0 || strchr(name, '/') != 0){
			printf("Incorrect filename\n");
			close(cd);
			close(sd);
			return __LINE__;
		}



		if(FileSize <= 0){
			printf("File is empty\n");
			close(cd);
			close(sd);
			return __LINE__;
		}
		if(NameLenght < 0){
			NameLenght = (-1)*(NameLenght);
		}
	



		FILE* file = fopen(name, "wb");

		char buf[1];
		for(int i = 0; i < FileSize; i++){
			if(recv(cd, buf, 1, 0) == 0){
				printf("Entered size is bigger than real size\n");
				close(cd);
				close(sd);
				fclose(file);
				remove(name);
				return __LINE__;
			}
			fwrite(buf, sizeof(char), 1, file);
		}
		if(recv(cd, buf, 1, 0) != 0){
			printf("Real size is bigger than entered size\n");
			close(cd);
			close(sd);
			fclose(file);
			remove(name);
			return __LINE__;
		}
		rewind(file);
		fclose(file);
		free(name);
		FileSize = 0;
		NameLenght = 0;
	}

	close(cd);
	close(sd);
	return 0;
}
