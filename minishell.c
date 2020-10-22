/*
*  ------------------------------------------------------------------------------
* |                          #PARA TENER EN CUENTA#                             |
* | Para poder ejercutar el minishell, es necesario tener instalada la libreria |
* | readline ejecutando en la terminal: sudo apt-get install libreadline-dev    |
* | Para compilar el minishell ejecutar:                                        |
* | gcc minishell.c -L/usr/include -lreadline -o minishell y luego ./minishell  |
* -------------------------------------------------------------------------------
* Asignatura: Sistemas operativos
* Archivo: Minishell.c
* Fecha de creación: 16-Octubre-2020
* Ultima modificación:18-Octubre-2020
* Versión: 1.11
* Autor de modificación: Lina Marcela Guamanga cod 1730223
*/

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>

#define MAXCOM 1000 //Maximo número de letras que pueden ser soportadas
#define MAXLIST 100 //Maximo número de comandos que pueden ser soportados

char cwd[1024]; //Aqui se guardara la dirección de ubicación
char minishell[1024]; // Aqui se guardara la dirección a ejecutar el minishell

//Función que recibe datos
int inputData(char* str){
    char* buf;
    buf = readline("\nprompt> ");
    if (strlen(buf) !=0){
	strcpy( str, buf);
	return 0;
    } else {
	return 1;
    }
}

//Función para obtener el minishell
void getMinishell(){
    getcwd(minishell, sizeof(minishell));
}

//Funcion para imprimir el directorio actual
void printDir(){
    getcwd(cwd, sizeof(cwd));
    printf("\nDirectory:%s MiniShell:%s\n", cwd, minishell);
}

//Funcion donde se ejecutan los comandos
void execArgs(char** parsed){
   pid_t pid = fork();
    if (pid < 0){
	printf("fork failed\n");
	return;
    } else if (pid == 0){
	execvp(parsed[0],parsed);
	exit(0);
    } else {
	wait(NULL);
	return;
    }
}

//Funcion que ejecuta los comandos de pipeline
void execArgsPiped(char** parsed, char** parsedpipe){
    int fd[2];
    pid_t pid;

    if (pipe(fd) <0){
        perror("Creating pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid <0){
        perror("fork() failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0){
        //The child process
        //Close the pipe write descriptor
        close(fd[1]);
        dup2(fd[0],STDIN_FILENO);
        close(fd[0]);
        execvp(parsedpipe[0],parsedpipe);
    } else {
        //The parent process
        //Close the pipe read descriptor
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        close(fd[1]);
        execvp(parsed[0],parsed);
	return;
    }
}

//Funcion que ejecuta los comandos de redireccionamiento
void execArgsRedir(char** parsed, char** parsedredir){
    int fd = open(parsedredir[0], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    if (fd <0){
	perror("open()");
	exit(EXIT_FAILURE);
    }

    close(STDOUT_FILENO);
    dup2(fd,STDOUT_FILENO);
    execvp(parsed[0],parsed);
    return;
}

//Funcion que limpia la pantalla
void clear(){
    const char* CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO,CLEAR_SCREEN_ANSI,12);
    return;
}

//Funcion para entrar a otra carpeta si existe
void cdDirectory(char** parsed){
    DIR *dir;
    dir = opendir(parsed[1]);
    if (dir == NULL){
	printf("Error opening directory\n");
    } else {
	chdir(parsed[1]);
    }
    return;
}

//Funcion de menú de ayuda
void help(){
    printf("\n--- WELCOME TO MY MINISHELL ---"
		"\n The list of available internal commands is:"
		"\n * cd <directory>: Change directory"
		"\n * environ: List all environment strings"
		"\n * pipe  | : Pipeline"
		"\n * clr: Clear screen"
		"\n * redir > : redirect"
		"\n * help: Display the user manual"
		"\n * exit: Exit Minishell");
}

//Funcion para encontrar el pipe "|"
int parsePipe(char* str, char** strpiped) {
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
	if (strpiped[i] == NULL){
            break;
	}
    }

    if (strpiped[1] == NULL){
        return 0; //Retorna cero si el pipeline no se encuentra
    } else {
        return 1;
    }
}

//Funcion para encontrar el ">"
int parseRedir(char* str, char** stredired) {
    int i;
    for (i = 0; i < 2; i++) {
        stredired[i] = strsep(&str, ">");
        if (stredired[i] == NULL){
            break;
        }
    }

    if (stredired[1] == NULL){
        return 0; //Retorna cero si el ">" no se encuentra
    } else {
        return 1;
    }
}

//Funcion para ejecutar los comandos
int commandsCreated(char** parsed){
    int total = 5, i, choice = 0;
    char* commands[total];

    commands[0] = "cd";
    commands[1] = "clr";
    commands[2] = "environ";
    commands[3] = "help";
    commands[4] = "exit";

    for (i=0; i<total; i++){
	if (strcmp (parsed[0], commands[i]) ==0){
	    choice = i+1;
	    break;
	}
    }

    switch (choice){
    case 1:
	cdDirectory(parsed);
	return 1;
    case 2:
	clear();
	return 1;
    case 3:
	printDir();
	return 1;
    case 4:
	help();
	return 1;
    case 5:
	exit(0);
	break;
    default:
	break;
    }
    return 0;

}

//Funcion que realiza el parsing de los comandos escritos
void parseSpace(char* str, char** parsed){
    int i;

    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");
        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}


int processString(char* str, char** parsed, char** parsedpipe, char** parsedredir){

    char* strpiped[2];
    char* stredired[2];
    int piped = 0;
    int redir = 0;
    piped = parsePipe(str, strpiped);
    redir = parseRedir(str, stredired);

    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);

    } else if(redir){
	parseSpace(stredired[0], parsed);
        parseSpace(stredired[1], parsedredir);
	redir = 2;
    } else {
        parseSpace(str, parsed);
    }

    if (commandsCreated(parsed))
        return 0;
    else
	return 1 + piped + redir;
}

int main(){

    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsedArgsPiped[MAXLIST];
    char* parsedArgsRedir[MAXLIST];
    int execFlag = 0;
    getMinishell();

    while (1) {
        //Toma los datos de entrada
        if (inputData(inputString))
            continue;
        // proceso
        execFlag = processString(inputString,
        parsedArgs, parsedArgsPiped, parsedArgsRedir);
        // execflag retorna cero si no esta el comando
        // 1 si es un simple comando
        // 2 si el comando incluye un pipe "|"
        // 3 si el comando es de redireccionamiento ">"

        if (execFlag == 1)
            execArgs(parsedArgs);

        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);

	if (execFlag == 3)
	    execArgsRedir(parsedArgs, parsedArgsRedir);
    }
    return 0;
}
