#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define SIZE 10000
#define SIZE2 100

int main(int argc, char *argv[]) {
  if (!argv[1]) {
    fprintf(stderr, "Enter port number!\n");
    return 1;
  }

  int sockfd1 = 0;
  int connfd1 = 0;

  struct sockaddr_in serv_addr = {0};

  char buff[SIZE] = "";
  char buff2[SIZE] = "";

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[1]));
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  sockfd1 = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd1 < 0) {
    perror("Error! ");
    return 1;
  }

  if (bind(sockfd1, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Error! ");
    return 1;
  }

  if (listen(sockfd1, 5) < 0) {
    perror("Error! ");
    return 1;
  }

  connfd1 = accept(sockfd1, (struct sockaddr *)NULL, NULL);

  if (connfd1 < 0) {
    perror("Error! ");
    return 1;
  }

  printf("Connection established...\n");

  recv(connfd1, buff, sizeof(buff), 0);
  recv(connfd1, buff2, sizeof(buff2), 0);

  int fd = open("text.txt", O_WRONLY);

  if (fd < 0) {
    printf("Error!\n");
  }
  puts("Data");
  write(fd, buff, strlen(buff));
  write(fd, buff2, strlen(buff2));

  close(sockfd1);
  return 0;
}