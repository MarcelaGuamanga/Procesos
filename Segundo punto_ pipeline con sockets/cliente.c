/*
* Asignatura: Sistemas Operativos
* Archivo: cliente.c
* Fecha de creación: 18-Octubre-2020
* Fecha ultima modificación: 18-Octubre-2020
* Autor: Lina Marcela Guamanga, cod: 1730223
* Version: 1.1
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
int main(int argc, char **argv){
  char *ip = "127.0.0.1";
  if(argc<1)
  {
    printf("<puerto>\n");
    return 1;
  }
  struct sockaddr_in cliente; //Declaración de la estructura con información para la conexión
  struct hostent *servidor; //Declaración de la estructura con información del host

  int puerto, conexion;
  char buffer[100];
  conexion = socket(AF_UNIX, SOCK_STREAM, 0); //Asignación del socket
  puerto=(atoi(argv[1])); //conversion del argumento
  bzero((char *)&cliente, sizeof((char *)&cliente)); //Rellena toda la estructura de 0's
        //La función bzero() es como memset() pero inicializando a 0 todas la variables
  cliente.sin_family = AF_UNIX; //asignacion del protocolo
  cliente.sin_port = htons(puerto); //asignacion del puerto
  cliente.sin_addr.s_addr=htonl(*ip);
  //bcopy((char *)servidor->h_addr, (char *)&cliente.sin_addr.s_addr, sizeof(servidor->h_length));
  //bcopy(); copia los datos del primer elemendo en el segundo con el tamaño máximo 
  //del tercer argumento.

  //cliente.sin_addr = *((struct in_addr *)servidor->h_addr); //<--para empezar prefiero que se usen
  //inet_aton(argv[1],&cliente.sin_addr); //<--alguna de estas dos funciones
  if(connect(conexion,(struct sockaddr *)&cliente, sizeof(cliente)) < 0)
  { //conectando con el host
    printf("Error conectando con el host\n");
    close(conexion);
    return 1;
  }
  printf("Conectado con %s:%d\n",inet_ntoa(cliente.sin_addr),htons(cliente.sin_port));
  //inet_ntoa(); está definida en <arpa/inet.h>
  printf("Escribe un mensaje: ");
  fgets(buffer, 100, stdin);
  send(conexion, buffer, 100, 0); //envio
  bzero(buffer, 100);
  recv(conexion, buffer, 100, 0); //recepción
  printf("%s", buffer);
return 0;
}
