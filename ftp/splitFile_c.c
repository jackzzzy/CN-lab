#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
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

  struct stat sb = {0};

  int sockfd1 = 0;
  int connfd1 = 0;

  struct sockaddr_in serv_addr = {0};

  char buffer[SIZE] = "";
  char filename[SIZE2] = "";

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[1]));
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // connecting to the first server (login)
  sockfd1 = socket(AF_INET, SOCK_STREAM, 0);

  // accepting a client
  connfd1 = connect(sockfd1, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  printf("Connection Established...\n");

  printf("\nEnter the filename: ");
  scanf("%s", filename);

  // open file and get length
  int fd = open(filename, O_RDONLY);

  if (fd < 0) {
    perror("");
    return 1;
  }

  fstat(fd, &sb);

  int length = sb.st_size;

  char b1[SIZE] = "";
  char b2[SIZE] = "";

  read(fd, b1, length / 2);
  pread(fd, b2, SIZE, length - length / 2);

  printf("\nOne:\n%s\n", b1);
  printf("Two:\n%s\n", b2);

  send(sockfd1, b1, length / 2, 0);
  send(sockfd1, b2, length - length / 2, 0);

  close(sockfd1);

  return 0;
}
