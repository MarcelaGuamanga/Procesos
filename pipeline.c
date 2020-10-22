/*--------------------------------------------------------------------
* |                     #PARA TENER EN CUENTA#                       |
* |   Para compilar pipeline.c ejecutar: gcc pipeline.c -o pipeline  |
* |   Luego: ./pipeline "cat p4.c" "wc -l" --> como ejemplo          |
* --------------------------------------------------------------------
* Asignatura: Sistemas Operativos
* Archivo: pipeline.c
* Fecha de creación: 10-Octubre-2020
* Fecha ultima modificación: 10-Octubre-2020
* Autor: Lina Marcela Guamanga, cod: 1730223
* Version: 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#define READ 0
#define WRITE 1
int
main(int argc, char *argv[])
{
    int fd[2];
    if (pipe(fd) == -1) {
	perror("Creating pipe");
	exit(EXIT_FAILURE);
    }

    int rc = fork();

    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);

    } else if (rc == 0) {
        // child (new process)

        close(fd[WRITE]);
	dup2(fd[READ], STDIN_FILENO);

	char *cadena2 = strtok(argv[2], " ");
	char *var21;
	char *var22;
	var21= cadena2;
	cadena2 = strtok(NULL, " ");
	var22= cadena2;

	execlp(var21, var21, var22,NULL);

    } else {
        // parent goes down this path (original process)
	close(fd[READ]);
	dup2(fd[WRITE], STDOUT_FILENO);

	char *cadena1 = strtok(argv[1], " ");
	char *var11;
	char *var12;

	var11= cadena1;
	cadena1 = strtok(NULL, " ");
	var12= cadena1;

        execlp(var11, var11, var12,NULL);
    }
   return 0;
}      

