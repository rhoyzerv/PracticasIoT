/* Estos son los ficheros de cabecera usuales */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BACKLOG 20 /* El numero de conexiones permitidas */

main( int argc, char *argv[])
{  char BUFFER[100];
   int  PORT =5001; /* El puerto que serabierto */
   char buf[100];
   int fd, fd2; /* los ficheros descriptores */


   struct sockaddr_in server; /* para la informaci de la direcci del servidor */
   struct sockaddr_in client; /* para la informaci de la direcci del cliente */
   int sin_size;

   if(argc < 2)
    {
      printf("utilice %s <Puerto> \n",argv[0]);
      exit(1);
    }
   /* A continuaci la llamada a socket() */
   if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) { 
     printf("error en socket()\n");
      exit(-1);
   }
  else
     PORT=atoi(argv[1]);

   server.sin_family = AF_INET;

   server.sin_port = htons(PORT);
   /* Recuerdas a htons() de la secci "Conversiones"? =) */
      
   server.sin_addr.s_addr = INADDR_ANY;
   /* INADDR_ANY coloca nuestra direcci IP automicamente */

   bzero(&(server.sin_zero),8);
   /* escribimos ceros en el reto de la estructura */
      
  puts("bind");
   /* A continuaci la llamada a bind() */
   if(bind(fd,(struct sockaddr*)&server,
           sizeof(struct sockaddr))==-1) {
      printf("error en bind() \n");
      exit(-1);
   }
  puts("listen");
   if(listen(fd,BACKLOG) == -1) {  /* llamada a listen() */
      printf("error en listen()\n");   
      exit(-1);
   }
 puts("ciclo");
  while(1) {
      sin_size=sizeof(struct sockaddr_in);
      puts("sizeof");
      /* A continuaci la llamada a accept() */
      if ((fd2 = accept(fd,(struct sockaddr *)&client, &sin_size))==-1) {
         printf("error en accept()\n");   
         exit(-1);
      }
      puts("aqui");
    //  printf("Se obtuvo una conexi desde %s\n", inet_ntoa(client.sin_addr) );
      /* que mostrarla IP del cliente */
      recv(fd2,BUFFER,100,0);
      
      if ( strcmp(BUFFER, "Help") == 0){
           send(fd2,"Selecciona un chiste usando algun numero!",100,0);
           }
        else if ( strcmp(BUFFER, "1") == 0) {
           send(fd2,"—Oye, ¿sabes cómo se llaman los habitantes de Barcelona?\n\n—Hombre, pues todos no.",100,0);
      }else if (strcmp(BUFFER, "2") == 0){
           send(fd2,"—¿Dónde vas, Antonio?\n\n—A por estiércol para las fresas.\n\n—¿Pero por qué no te las comes con nata, como todo el mundo?",100,0);
      }else if ( strcmp(BUFFER, "3") == 0) {
           send(fd2,"—Doctor, tengo todo el cuerpo cubierto de pelo. ¿Qué padezco?\n\n——Padece uzté un ozito.",100,0);
      } else if ( strcmp(BUFFER, "4") == 0) {
           send(fd2,"—Hombre, Juan, cuánto tiempo. ¿Dónde vives ahora?\n\n—En Leganés.\n\n—Qué bien, donde el monstruo.",100,0);
      } else if ( strcmp(BUFFER, "4") == 0) {
           send(fd2,"¿Cuál es el peinado favorito de los carteros?\n\nLos tirabuzones.",100,0); 
      }else {
           send(fd2,"Proximamente",100,0);
      }
      
      close(fd2); /* cierra fd2 */
   }
}

