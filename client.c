#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <ncurses.h>

#define ERROR(msg) \
  endwin(); \
  perror(msg); \
  exit(1);

#define ERROR_IF(b, msg) \
  if (b) {\
    ERROR(msg); \
  }

int main(int argc, char *argv[]) 
{
  initscr(); // Init curses
  cbreak();  // Don't wait for EOL

  int err = 0, i = 0;
  char recvBuff[1024] = { 0 };
  struct sockaddr_in serv_addr;

  if (argc != 2)
  {
    printf("\n Usage: %s <Server IP>\n", argv[0]);
    endwin();
    return 1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(5000);

  err = inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
  ERROR_IF(err <= 0, "Error: inet_pton() failed")

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  ERROR_IF(sockfd < 0, "Error: Could not create socket")

  err = connect(sockfd, (const struct sockaddr*)&serv_addr, sizeof(serv_addr));
  ERROR_IF(err < 0, "Error: connect() failed")

  char ipstr[INET_ADDRSTRLEN]; 
  inet_ntop(AF_INET, &serv_addr, ipstr, sizeof ipstr);
  printw("Connected to (%s : 5000)\n", ipstr);
  refresh();

  while (1)
  {
    char c = getch();
    switch (c) 
    {
      default:
        break;
      case '1':
      case '2':
      case 'a': 
      case 'o': {
        err = send(sockfd, &c, 1, 0);
        ERROR_IF(err != 1, "Error: send() didn't send all data")
        ERROR_IF(err < 0, "Error: send() failed")
        break;
      }
      case 'q': {
        err = send(sockfd, &c, 1, 0);
        ERROR_IF(err != 1, "Error: send() didn't send all data")
        ERROR_IF(err < 0, "Error: send() failed")
        ERROR("Exit");
      }
    }
  }

  endwin();   // End curses
  return 0;
}
