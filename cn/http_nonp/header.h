#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PORT 3232
#define SIZE 300
#define N 20

typedef struct {
  int regno;
  int age;
  int sem;
  int marks[5];
  char name[SIZE];
  char gender;
} student;

typedef struct {
  char method[N];
  char path[N];
  char version[N];
  char accept[N];
  char connection[N];
  char useragent[N];
  char req;
  student stu;
} request;

typedef struct {
  int status;
  char statmsg[N];
  char version[N];
  char connection[N];
  char contype[N];
  char datetime[N];
  char answer[6][SIZE];
} response;
