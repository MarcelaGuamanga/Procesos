Para ejecutar el pipeline con sockets, es necesario abrir dos consolas desde su computador, ya que se conectan localmente.
Luego desde una consola compilar el archivo servidor.c  as�:
gcc servidor.c -o servidor
Ejecutarlo: ./servidor <puerto> 
Ejemplo: ./servidor 80

De la misma manera en la segunda consola compilar el archivo cliente.c as�:
gcc cliente.c -o cliente
Ejecutarlo: ./cliente <puerto>
Ejemplo: ./cliente 80

Dentro del cliente dar la instrucci�n de pipeline que recibir� el servidor, ejemplo: ls wc
El servidor dar� una respuesta de �Recibido� arrojando la respuesta del comando.

PDT: No s� porque no me da es como si no reconociera el comando wc
