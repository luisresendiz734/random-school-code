#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

/*Servidor*/

int main()
{
char cadena[100]; //creamos un arreglo para guardar los mensajes que recibamos
int listen_fd, comm_fd; //la primera es para crear el socket y la segunda para recoger a los clientes de la lista de espera
struct sockaddr_in servaddr;
FILE *myf= fopen("Conversacion_servidor.txt", "a"); //apuntador tipo file para guardar nuestra conversacion en un archivo txt
time_t t; //las variables tipo time nos permitira visualizar la hora la hora a la que se manden los mensajes
struct tm *tm;
char hora[100];
char *tmp;
char sendline[100]="Usando el puerto 22000\n"; //arreglo donde escribiremos el mensaje


//Sockets
listen_fd=socket(AF_INET, SOCK_STREAM, 0); //usamos la variable entera que creamos y le asignamos la variable socket, su primer variable nos permite entablar conversacion con otros ordenadores, la segunda paraindicar que el socket es orientado a conexion y el 0.
bzero(&servaddr, sizeof(servaddr)); //para asegurarnos que el socket este vacio lo llenaremos con ceros, para ello usamos esta funcion

//Estructura del socket
servaddr.sin_family=AF_INET;//permite la comunicacion entre los ordenadores
servaddr.sin_addr.s_addr=htons(INADDR_ANY);//Nos permite atender a cualquier cliente que quiera conectarse al servidor
servaddr.sin_port =htons(22000);//Puerto por el que establecera la comunicacion

bind(listen_fd,(struct sockaddr *)&servaddr, sizeof(servaddr));//indicar al sistema que tenemos abierto el socket y atenderemos el servicio
listen(listen_fd, 10);//indicamos que vamos a atender las llamadas de los clientes
comm_fd=accept(listen_fd, (struct sockaddr*)NULL, NULL); //aceptamos la llamada del cliente


//Comenzamos el chat
printf("\n\n------Inicio el chat EQUIPO3------\n\n");
printf("\n\n------Inicio el chat EQUIPO3------\n\n", myf);

while(!strstr(cadena, "Hasta luego")&&!strstr(sendline, "Hasta luego"))//mientras que el usuario no escriba hasta luego seguiremos con la comunicacion
	{
		bzero(cadena, 100);//vaciamos la cadena recibida llenandola de ceros
		t=time(NULL);//Para ver la hora en que se recibio un mensaje
		tm=localtime(&t);
		strftime(hora, 100, "\nCliente(%H:%M)->", tm);
		
		read(comm_fd, cadena, 100);//llenamos la cadena
		tmp=strcat(hora, cadena);
		printf("%s", tmp);//imprimimos la cadena en la consola
		fputs(tmp, myf);//Para guardar nuestra conversacion en un archivo de texto con el apuntador file que creamos al inicio
		if(!strstr(cadena, "Hasta luego")){//si el mensaje no es hasta luego, deberemos responder el mensaje
			strftime(hora, 100, "\nYo(Servidor) (%H:%M)->", tm);
			printf("%s", hora);//imprimimos la hora del envio del mensaje
			gets(sendline);//y leemos lo que ingresamos en la consola
			tmp=strcat(hora, sendline);//se encia el mensaje al presionar enter
			write(comm_fd, sendline, strlen(sendline)+1); //para enviar el mensaje
			fputs(tmp, myf);//escribimos el mensaje en el archivo de texto
		}
	}
	printf("\n\nConversacion finalizada, gracias por usar nuestro chat :3\n");//imprimimos que la conversacion termino
	printf("\nSe genero el archivo -> Conversacion_servidor.txt\n"); 
	//cerramos el archivo de texto con la funcion fclose
	fclose(myf);
}
