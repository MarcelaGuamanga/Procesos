/*
* Asignatura: Sistemas Operativos
* Archivo: servidor.C
* Fecha de creación: 18-Octubre-2020
* Fecha ultima modificación: 19-Octubre-2020
* Autor: Lina Marcela Guamanga, cod: 1730223
* Version: 1.3
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/types.h>
#include<errno.h>

void pipeIdea(char argv[]){

     char *cadena = strtok(argv, " ");
     char *var1;
     char *var2;
     var1= cadena;
     cadena = strtok(NULL, " ");
     var2= cadena;

     int fd[2];
     int pid;

     if (pipe(fd) == -1) {
        perror("Creating pipe");
        exit(EXIT_FAILURE);
     }

     pid = fork();

     if (pid <0){
	perror("fork() failed");
	exit(EXIT_FAILURE);

     } else if(pid == 0){
	printf("hola soy el hijo, mi variable:%s\n",var2);
        close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	execlp(var2, var2, NULL);
     } else {
	printf("hola soy el padre, mi varibale:%s\n", var1);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	execlp(var1, var1, NULL);
	return;
     }
}

int main(int argc, char **argv){
  char *ip = "127.0.0.1";
  if(argc<2)
  { //Especifica los argumentos
    printf("%s <puerto>\n",argv[0]);
    return 1;
  }
  int conexion_servidor, conexion_cliente, puerto; //declaramos las variables
  socklen_t longc; //Debemos declarar una variable que contendrá la longitud de la estructura
  struct sockaddr_in servidor, cliente;
  char buffer[100]; //Declaramos una variable que contendrá los mensajes que recibamos
  puerto = atoi(argv[1]);
  if((conexion_servidor = socket(AF_UNIX,SOCK_STREAM,0)) < 0){
   fprintf(stderr, "Error al crear el socket\n");
   return 1;
  }
  bzero((char *)&servidor, sizeof(servidor)); //llenamos la estructura de 0's
  servidor.sin_family = AF_UNIX; //asignamos a la estructura
  servidor.sin_port = htons(puerto);
  servidor.sin_addr.s_addr = htonl(*ip); //esta macro especifica nuestra dirección
  if(bind(conexion_servidor, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
  { //asignamos un puerto al socket
    printf("Error al asociar el puerto a la conexion\n");
    close(conexion_servidor);
    return 1;
  }
  listen(conexion_servidor, 3); //Estamos a la escucha
  printf("A la escucha en el puerto %d\n", ntohs(servidor.sin_port));
  longc = sizeof(cliente); //Asignamos el tamaño de la estructura a esta variable
  conexion_cliente = accept(conexion_servidor, (struct sockaddr *)&cliente, &longc); //Esperamos una conexion
  if(conexion_cliente<0)
  {
    printf("Error al aceptar trafico\n");
    close(conexion_servidor);
    return 1;
  }
  printf("Conectando con %s:%d\n", inet_ntoa(cliente.sin_addr),htons(cliente.sin_port));
  if(recv(conexion_cliente, buffer, 100, 0) < 0)
  { //Comenzamos a recibir datos del cliente
    //Si recv() recibe 0 el cliente ha cerrado la conexion. Si es menor que 0 ha habido algún error.
    printf("Error al recibir los datos\n");
    close(conexion_servidor);
    return 1;
  }
  else
  {

    send(conexion_cliente, "Recibido\n", 13, 0);
    pipeIdea(buffer);
    close(conexion_servidor);
    return 0;
  }
}
