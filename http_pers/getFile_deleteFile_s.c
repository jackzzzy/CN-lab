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
#define backlog 10
#define size 10000

struct header {
  char filename[size];
  char method[size];
  int length;
  time_t last_modified;
  time_t rest_time;
  char msg[size];
};
void http_persistent(int client) {
  while (1) {
    struct header x, y;
    recv(client, (struct header *)&y, sizeof(struct header), 0);
    if (strcmp(y.method, "exit") == 0)
      break;
    printf("\nThe file name is: %s\n", y.filename);
    printf("\nThe method is: %s\n", y.method);
    if (strcmp(y.method, "GET") == 0) {
      FILE *fp = fopen(y.filename, "r");
      strcpy(x.method, "GET");
      time(&x.rest_time);
      if (fp == NULL) {
        x.length = 0;
        strcpy(x.msg, " 404 Not found - The file does not exit!!");
      } else {
        time(&x.last_modified);
        int c, i = 0;
        while ((c = fgetc(fp)) != EOF)
          x.msg[i++] = c;
        x.msg[i] = '\0';
        x.length = i;
        printf("Length of file is %d\n", x.length);
      }
    } else if (strcmp(y.method, "DELETE") == 0) {
      FILE *fp = fopen(y.filename, "r");
      if (fp == NULL) {
        strcpy(x.msg, "404-Not found");
      } else {
        if (remove(y.filename))
          strcpy(x.msg, "The file not deleted");
        else
          strcpy(x.msg, "The file deleted Successfully");
      }
    } else {
      time(&x.rest_time);
      x.length = 0;
      strcpy(x.msg, "400 Bad request");
    }
    send(client, (void *)&x, sizeof(struct header), 0);
  }
}
void test(int client) {
  char a[size];
  recv(client, &a, sizeof(a), 0);
  bzero(a, size);
  strcpy(a, "Connection is good");
  send(client, &a, size, 0);
}
int main(int argc, char *argv[]) {
  int port = atoi(argv[1]);
  int server;
  server = socket(domain, type, protocol);
  if (server == -1) {
    printf("\nError is creating socket at server side\n");
    exit(0);
  } else
    printf("\nServer socket created successfully\n");
  struct sockaddr_in serv_addr, cli;
  serv_addr.sin_family = domain;
  serv_addr.sin_port = htons(port);
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  if (bind(server, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    printf("\nError in Binding the port with the socket\n");
    close(server);
    exit(0);
  } else
    printf("\nBinded to the port successfully\n");
  if (listen(server, backlog) == -1) {
    printf("\nError in listening to client\n");
    close(server);
    exit(0);
  } else
    printf("\nListening to client\n");
  char msg[size], buff[size];
  strcpy(msg, "\nConnected to server succesfully\n");
  int len = sizeof(cli);
  int client = accept(server, (struct sockaddr *)&cli, &len);
  if (client == -1) {
    printf("\nError in accepting request\n");
    close(server);
    exit(0);
  } else
    printf("\nAccepted the request successsully\n");
  // test (client);
  http_persistent(client);
  close(server);
  return 0;
}
