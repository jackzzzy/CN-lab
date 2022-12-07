#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
 
int main(int argc, char* argv[]) {
        int clientSocket,result;
        char buffer[1024];
        struct sockaddr_in serverAddr;
        socklen_t addr_size;
 
        clientSocket = socket(PF_INET, SOCK_STREAM, 0);
 
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(atoi(argv[1]));
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 
        addr_size = sizeof serverAddr;
        connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
        while(1)
        {
                printf("enter a message:\n");
re:
                scanf("%s",buffer);
 
                send(clientSocket,buffer,13,0);
 
 
                recv(clientSocket,buffer,1024,0);
                result=strcmp(buffer,"stop");
 
 
                if(result==0)
                {
                        printf("%s\n",buffer);
                        goto re;
                }
                printf("Received from server: %s\n",buffer);
        }
        return 0;
}
