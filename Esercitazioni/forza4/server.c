#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <errno.h>

typedef struct
{
    char messaggio[20];
    int colonna;
    char nome[20];
    char valore;
    char matrix[6][7];
} Messaggio;

int InserisciValore(Messaggio *m)
{
    int numc = m->colonna;
    for (int i = 5; i >= 0; i--)
    {
        if (m->matrix[i][numc] == ' ')
        {
            m->matrix[i][numc] = m->valore;
            // printf("Ho inserito %c in posizione %d %d\n", m->valore, i, numc);
            return i;
        }
    }
}

bool Verifica(Messaggio m, int rig)
{
    int posx = rig, posy = m.colonna;
    int cont = 0;
    while (posx != 0 && posy != 0)
    {
        posx--;
        posy--;
    }

    while (posx != 5 && posy != 6)
    {
        if (m.matrix[posx][posy] == m.valore)
        {
            cont++;
        }
        else
        {
            cont = 0;
        }
        if (cont >= 4)
        {
            return true;
        }
        posx++;
        posy++;
    }

    printf("Conteggio prima diagonale con Riga : %i Colonna : %i Conteggio : %i \n", posx, posy, cont);

    posx = rig, posy = m.colonna;

    while (posx != 0 && posy != 6)
    {
        posx--;
        posy++;
    }
    cont = 0;
    while (posx != 6 && posy != -1)
    {

        if (m.matrix[posx][posy] == m.valore)
        {
            cont++;
        }
        else
        {
            cont = 0;
        }
        if (cont >= 4)
        {
            return true;
        }
        posx++;
        posy--;
    }

    printf("Conteggio prima diagonale con Riga : %i Colonna : %i Conteggio : %i \n", posx, posy, cont);
    return false;
}

int main(int argc, char **argv)
{
    int sockfd, n;
    extern int errno;
    struct sockaddr_in local_addr, client1, client2;
    socklen_t len = sizeof(struct sockaddr_in);
    char mesg[1000];

    if (argc < 2)
    {
        printf("Use: server listening_PORT");
        return 0;
    }

    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\nError in socket opening ... exit!");
        return -1;
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(atoi(argv[1]));
    if (bind(sockfd, (struct sockaddr *)&local_addr, len) < 0)
    {
        printf("\nBinding error! Error code n.%d \n", errno);
        return -1;
    }
    Messaggio m;

    recvfrom(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&client1, &len);
    printf("[CLIENT 1] - %s \n", m.nome);
    sendto(sockfd, "attesa", strlen("attesa"), 0, (struct sockaddr *)&client1, len);
    recvfrom(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&client2, &len);
    printf("[CLIENT 2] - %s \n", m.nome);
    sendto(sockfd, "ok", strlen("ok"), 0, (struct sockaddr *)&client2, len);
    sendto(sockfd, "ok", strlen("ok"), 0, (struct sockaddr *)&client1, len);
    memset(&m.matrix, ' ', sizeof(m.matrix));
    bool cond = false;
    int riga = -1;
    for (;;)
    {
        sendto(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&client1, len);

        printf("Turno Client 1...\n");
        sendto(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&client1, len); // Gioca il client 1
        recvfrom(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&client1, &len);
        riga = InserisciValore(&m);
        /*
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
    */
        if (Verifica(m, riga))
        {
            strcpy(m.messaggio, "Hai vinto!");
            sendto(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&client1, len);
            strcpy(m.messaggio, "Hai perso!");
            sendto(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&client2, len);
            break;
        }
        else
        {
            sendto(sockfd, "Colonna inserita correttamente!", strlen("Colonna inserita correttamente!"), 0, (struct sockaddr *)&client1, len);
        }

        sendto(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&client2, len); // Invio matrice
        printf("Turno Client 2...\n");
        sendto(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&client2, len); // Gioca il client 2
        recvfrom(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&client2, &len);
        riga = InserisciValore(&m);
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
        if (Verifica(m, riga))
        {
            strcpy(m.messaggio, "Hai vinto!");
            sendto(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&client2, len);
            strcpy(m.messaggio, "Hai perso!");
            sendto(sockfd, &m, sizeof(m), 0, (struct sockaddr *)&client1, len);
            break;
        }
        else
        {
            sendto(sockfd, "Colonna inserita correttamente!", strlen("Colonna inserita correttamente!"), 0, (struct sockaddr *)&client2, len);
        }
    }
}
