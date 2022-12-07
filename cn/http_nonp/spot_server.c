#include "header.h"

int main(int argc, char *argv[]) {
  response sr[2];
  strcpy(sr[0].version, "http/1.1");
  strcpy(sr[0].connection, "keep-alive");
  strcpy(sr[0].contype, "text");
  strcpy(sr[1].version, "http/1.1");
  strcpy(sr[1].connection, "keep-alive");
  strcpy(sr[1].contype, "text");
  struct sockaddr_in serveraddr;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(PORT);
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) return 0;
  if (listen(sockfd, 2) < 0)
    return 0;
  printf("Server listening on port: %d\n", PORT);
  int clisock = accept(sockfd, NULL, NULL);
  if (clisock < 0)
    return 0;
  while (1) {
    request clr[2];
    int len = recv(clisock, &clr[0], sizeof(clr[0]), 0);
    printf("CLIENT'S REQUEST:\n%s%s%s\nConnection: %s\nAccept:%s\nUser-Agent: %s\nRequired sem: %d\nRequest: %c\n", clr[0].method, clr[0].path, clr[0].version, clr[0].connection, clr[0].accept, clr[0].useragent, clr[0].stu.sem, clr[0].req);
    if (clr[0].stu.sem == -1) break;
    len = recv(clisock, &clr[1], sizeof(clr[1]), 0);
    printf("CLIENT'S REQUEST:\n%s%s%s\nConnection: %s\nAccept:%s\nUser-Agent: %s\nRequired sem: %d\nRequest: %c\n", clr[1].method, clr[1].path, clr[1].version, clr[1].connection, clr[1].accept, clr[1].useragent, clr[1].stu.sem, clr[1].req);
    if (clr[1].stu.sem == -1) break;
    FILE *inf;
    inf = fopen("stud", "rb");
    student s;
    int maxmark[] = {0, 0, 0, 0, 0, 0};
    while (fread(&s, sizeof(student), 1, inf) == 1) {
      if (s.sem == clr[0].stu.sem) {
        for (int i = 0; i < 5; i++) {
          if (s.marks[i] > maxmark[i]) {
            maxmark[i] = s.marks[i];
            strcpy(sr[0].answer[i], s.name);
            strcpy(sr[0].statmsg, "OK");
            sr[0].status = 200;
          }
        }
      }
      if ((s.marks[0] + s.marks[1] + s.marks[2] + s.marks[3] + s.marks[4] > maxmark[5]) && (s.sem == clr[1].stu.sem)) {
        maxmark[5] = s.marks[0] + s.marks[1] + s.marks[2] + s.marks[3] + s.marks[4];
        strcpy(sr[1].answer[5], s.name);
        strcpy(sr[1].statmsg, "OK");
        sr[1].status = 200;
      }
    }
    fclose(inf);
    send(clisock, &sr, sizeof(sr), 0);
  }
  close(sockfd);
  return 0;
}
