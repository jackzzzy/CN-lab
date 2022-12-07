#include "header.h"

int main(int argc, char *argv[]) {
  request cr;
  strcpy(cr.method, "GET");
  strcpy(cr.path, "/");
  strcpy(cr.version, "http/1.1");
  strcpy(cr.connection, "keep-alive");
  strcpy(cr.accept, "text");
  struct sockaddr_in cliaddr;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  cliaddr.sin_family = AF_INET;
  cliaddr.sin_port = htons(PORT);
  cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (connect(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0)
    return 0;
  printf("Connection made with the server.\n");
  printf("Enter useragent: ");
  scanf("%s", cr.useragent);
  char c;
  while (1) {
    printf("Enter semester [-1 to exit]: ");
    scanf("%d", &cr.stu.sem);
    if (cr.stu.sem == -1) {
      cr.req = 'e';
      send(sockfd, &cr, sizeof(cr), 0);
      break;
    }
    printf("Enter request type ['a' for subject toppers or 'b' for overall topper]: ");
    scanf("%s", &cr.req);
    send(sockfd, &cr, sizeof(cr), 0);
    printf("Enter semester [-1 to exit]: ");
    scanf("%d", &cr.stu.sem);
    if (cr.stu.sem == -1) {
      cr.req = 'e';
      send(sockfd, &cr, sizeof(cr), 0);
      break;
    }
    printf("Enter request type ['a' for subject toppers or 'b' for overall topper]: ");
    scanf("%s", &cr.req);
    send(sockfd, &cr, sizeof(cr), 0);
    response sr[2];
    int len = recv(sockfd, &sr, sizeof(sr), 0);
    if (len > 0) {
      printf("SERVER'S RESPONSE: \n%s\n%d%s\nConnection: %s\nContent-Type: %s\n\n", sr[0].version, sr[0].status, sr[0].statmsg, sr[0].connection, sr[0].contype);
      for (int i = 0; i < 5; i++)
        printf("Topper in subject #%d : %s\n", i, sr[0].answer[i]);
      printf("SERVER'S RESPONSE: \n%s\n%d%s\nConnection: %s\nContent-Type: %s\n\n", sr[1].version, sr[1].status, sr[1].statmsg, sr[1].connection, sr[1].contype);
      printf("Overall Topper : %s\n", sr[1].answer[5]);
    }
  }
  close(sockfd);
  printf("Disconnected from the server.\n");
  return 0;
}
