/* Sample fork */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char**argv)
{
   int n=0, pid;
   
   for(; n<5; ++n)
     printf("Processo principale %d \n",n);
    
   pid = fork();
    
   if (pid == 0)
   { for(; n<10; ++n)
     {
        sleep(1);
        printf("Processo figlio %d %d \n", pid, n*2);
     }   
     return 0;
   }
   else
   { for(; n<10; ++n)
     {
       sleep(1);
       printf("Processo padre %d %d \n", pid, n*3);
     } 
     return 0;
   }
         
}
