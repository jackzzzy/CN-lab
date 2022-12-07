#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#define domain AF_INET
#define type SOCK_STREAM
#define protocol 0
#define size 10000
struct header {
  char filename[size];
  char method[size];
  int length;
  time_t last_modified;
  time_t res_time;
  char msg[size];
};
void http_persistent(int client) {
  while (1) {
    struct header x, y;
    printf("\nEnter the method or enter exit to close the connections\n");
    scanf("%s", &x.method);
    if (strcmp(x.method, "exit") == 0) {
      send(client, (void*)&x, sizeof(struct header), 0);
      break;
    }
    printf("Enter the URL(filename): ");
    scanf("%s", &x.filename);
    send(client, (void*)&x, sizeof(struct header), 0);
    recv(client, (struct header*)&y, sizeof(struct header), 0);
    printf("The message is \n");
    fputs(y.msg, stdout);
    printf("The response time is %s\n", ctime(&y.res_time));
  }
}
void test(int client) {
  char a[size];
  strcpy(a, "Server is good\n");
  send(client, &a, size, 0);
  bzero(a, size);
  recv(client, &a, sizeof(a), 0);
  printf("The message is %s\n", a);
}
int main(int argc, char* argv[1]) {
  int port = atoi(argv[1]);
  int client;
  client = socket(domain, type, protocol);
  if (client == -1) {
    printf("Error in creating socket at client side\n");
    exit(0);
  } else
    printf("Client Socket created successfully\n");
  struct sockaddr_in client_addr;
  client_addr.sin_family = domain;
  client_addr.sin_port = htons(port);
  client_addr.sin_addr.s_addr = INADDR_ANY;
  int server = connect(client, (struct sockaddr*)&client_addr, sizeof(client_addr));
  if (server == -1) {
    printf("Error in connecting to server\n");
    close(client);
    exit(0);
  } else
    printf("Connected to server\n");
  // test client
  http_persistent(client);
  close(client);
  return 0;
}
