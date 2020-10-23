Para ejecutar el pipeline con sockets, es necesario abrir dos consolas desde su computador, ya que se conectan localmente.
Luego desde una consola compilar el archivo servidor.c  así:
gcc servidor.c -o servidor
Ejecutarlo: ./servidor <puerto> 
Ejemplo: ./servidor 80

De la misma manera en la segunda consola compilar el archivo cliente.c así:
gcc cliente.c -o cliente
Ejecutarlo: ./cliente <puerto>
Ejemplo: ./cliente 80

Dentro del cliente dar la instrucción de pipeline que recibirá el servidor, ejemplo: ls wc
El servidor dará una respuesta de “Recibido” arrojando la respuesta del comando.

PDT: No sé porque no me da es como si no reconociera el comando wc
