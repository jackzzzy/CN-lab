#include "header.h"
int main(int argc, char* argv[]) {
  int sockfd1, connfd1 = 0, sockfd2, connfd2 = 0;
  struct sockaddr_in serv_addr;
  char buffer[size];
  char file[100], user[100], pass[100], new[10];
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[1]));
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd1 < 0) {
    printf("Socket creation failed\n");
    return 1;
  }
  connfd1 = connect(sockfd1, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if (connfd1 < 0) {
    printf("server accept failed\n");
    return 1;
  } else
    printf("connection established\n");
  while (1) {
    printf("\nEnter the username: ");
    scanf("%s", user);
    send(sockfd1, user, strlen(user) + 1, 0);
    printf("Enter the password: ");
    scanf("%s", pass);
    send(sockfd1, pass, strlen(pass) + 1, 0);
    recv(sockfd1, buffer, sizeof(buffer), 0);
    if ((strncmp(buffer, "valid", strlen("valid"))) == 0) {
      printf("valid\n");
      break;
    } else
      printf("invalid: try once again\n");
  }
  serv_addr.sin_port = htons(atoi(argv[2]));
  sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd2 < 0) {
    printf("Socket creation failed\n");
    return 1;
  }
  connfd2 = connect(sockfd2, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if (connfd2 < 0) {
    printf("server accept failed\n");
    return 1;
  } else
    printf("connection established\n");
  while (1) {
    printf("\nEnter the filename('exit' to end): ");
    scanf("%s", file);
    if ((strncmp(file, "exit", strlen("exit"))) == 0) {
      send(sockfd2, "exit", strlen("exit") + 1, 0);
      break;
    }
    send(sockfd2, file, strlen(file) + 1, 0);
    recv(sockfd2, buffer, sizeof(buffer), 0);
    if ((strncmp(buffer, "found", strlen("foound"))) == 0) {
      printf("requested file found\n");
      printf("enter 'yes' to request file content(or enter 'no'):");
      scanf("%s", new);
      send(sockfd2, new, strlen(new) + 1, 0);
      if ((strncmp(new, "yes", strlen("yes"))) == 0) {
        recv(sockfd2, buffer, sizeof(buffer), 0);
        char new_file[size];
        int k = 0;
        int i = strlen(file) - 1;
        while (i > 0 && file[i] != '/') {
          i--;
        }
        for (int j = i; j < strlen(file); j++, k++) {
          new_file[k] = file[j];
        }
        new_file[k] = '_';
        new_file[k + 1] = 'c';
        new_file[k + 2] = 'o';
        new_file[k + 3] = 'p';
        new_file[k + 4] = 'y';
        new_file[k + 5] = '\0';
        printf("%s", new_file);
        FILE* fp = fopen(new_file, "w");
        fwrite(buffer, 1, strlen(buffer), fp);
        printf("content of the file: \n%s", buffer);
      }
    } else
      printf("file not found. try again\n");
  }
  close(sockfd1);
  close(sockfd2);
  return 0;
}
