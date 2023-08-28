#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

typedef struct
{
  char messaggio[20];
  int colonna;
  char nome[20];
  char valore;
  char matrix[6][7];
} Messaggio;

int main(int argc, char **argv)
{
  int sockfd, n;
  struct sockaddr_in remote_addr;
  char sendline[1000];
  char recvline[1000];
  socklen_t len = sizeof(struct sockaddr_in);

  if (argc != 3)
  {
    printf("usage:  UDPclient <remote_IP remote_port>\n");
    return 1;
  }

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    printf("\nError in socket opening ... exit!");
    return -1;
  }

  memset(&remote_addr, 0, len);
  remote_addr.sin_family = AF_INET;
  inet_pton(AF_INET, argv[1], &(remote_addr.sin_addr));
  remote_addr.sin_port = htons(atoi(argv[2]));

  Messaggio m;
  m.valore = 'x';
  m.colonna = -1;
  memset(m.nome, 0, 20);

  printf("Inserisci il tuo nome : ");
  scanf("%s", &m.nome);

  sendto(sockfd, &m, sizeof(m), 0,
         (struct sockaddr *)&remote_addr, len);
  n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *)&remote_addr, &len);
  recvline[n] = 0;
  printf("[SERVER] - %s \n", recvline);
  if (strcmp(recvline, "attesa") == 0)
  {
    n = recvfrom(sockfd, recvline, 999, 0, (struct sockaddr *)&remote_addr, &len);
    recvline[n] = 0;
  }
  printf("---Inizia il gioco---\n");
  for (;;)
  {
    recvfrom(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&remote_addr, &len);
    if ((strcmp(m.messaggio, "Hai vinto!") == 0) || (strcmp(m.messaggio, "Hai perso!") == 0))
    {
      printf("[SERVER] - %s \n", m.messaggio);
      break;
    }
    for (int i = 0; i < 6; i++)
    {
      printf("|");
      for (int j = 0; j < 7; j++)
      {
        printf("%c ", m.matrix[i][j]);
        printf("|");
      }
      printf("\n");
    }
    recvfrom(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&remote_addr, &len);
    printf("Inserisci la colonna : ");
    m.valore = 'x';
    scanf("%i", &m.colonna);

    sendto(sockfd, &m, sizeof(m), 0,
           (struct sockaddr *)&remote_addr, len);
    recvfrom(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&remote_addr, &len);
    printf("[SERVER] - %s \n", m.messaggio);
    if ((strcmp(m.messaggio, "Hai vinto!") == 0) || (strcmp(m.messaggio, "Hai perso!") == 0))
    {
      break;
    }
    printf("Attenti il tuo turno...\n");
  }
}
