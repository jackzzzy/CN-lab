#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
 
int main(int argc, char* argv[]){
        int welcomeSocket, newSocket,result,result1,result2,result3;
        char buffer[1024];
        struct sockaddr_in serverAddr;
        struct sockaddr_storage serverStorage;
        socklen_t addr_size;
 
        welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
 
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(atoi(argv[1]));
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 
        bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
 
        if(listen(welcomeSocket,5)==0)
                printf("Listening\n");
        else
                printf("Error\n");
 
        addr_size = sizeof serverStorage;
        newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
 
        while(1)
        {
r:
                recv(newSocket,buffer,1024,0);
                result=(buffer[0]=='0') ? 0 : 1;//strcmp(buffer,"debmalya");
                result1=strcmp(buffer,"stop");
                if(result1==0)
                {strcpy(buffer,"Resend");
                        send(newSocket,buffer,100,0);
 
                        goto r; }
                if(result==0) 
                {
                        printf("%s\n",buffer);
                        strcpy(buffer,"ack");
                        send(newSocket,buffer,13,0);
                }
                else                                                                                                                                              
                {                                                                                                                                                 
                        strcpy(buffer,"nack");                                                                                                                    
                        send(newSocket,buffer,13,0);                                                                                                              
                }                                                                                                                                                 
        }                                                                                                                                                         
        return 0;                                                                                                                                                 
}
