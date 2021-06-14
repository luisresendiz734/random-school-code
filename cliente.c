#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

int main()
{
char cadena[100]; //creamos un arreglo para guardar los mensajes que recibamos
int sock_fd; //contendra al socket del servidor
struct sockaddr_in servaddr;
FILE *myf= fopen("Conversacion_cliente.txt", "a"); //apuntador tipo file para guardar nuestra conversacion en un archivo txt
time_t t; //las variables tipo time nos permitira visualizar la hora la hora a la que se manden los mensajes
struct tm *tm;
char hora[100];
char *tmp;
char sendline[100]="Usando el puerto 22000"; //arreglo donde escribiremos el mensaje


sock_fd=socket(AF_INET, SOCK_STREAM, 0);
bzero(&servaddr, sizeof(servaddr));

servaddr.sin_family=AF_INET;
servaddr.sin_port=htons(22000);

inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));//usamos la direccion ip del servidor, usamos la 127.0.0.1 para comunicarnos con nuestro mismo equipo y mostrar los resultados
connect(sock_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));// para conectarnos al servidor

//Comenzamos el chat
printf("\n\n------Inicio el chat EQUIPO3------\n\n");
printf("\n\n------Inicio el chat EQUIPO3------\n\n", myf);

while(!strstr(cadena, "Hasta luego")&&!strstr(sendline, "Hasta luego"))//mientras que el usuario no escriba hasta luego seguiremos con la comunicacion
	{
		bzero(cadena, 100);//vaciamos la cadena recibida llenandola de ceros
		t=time(NULL);//Para ver la hora en que se recibio un mensaje
		tm=localtime(&t);
		strftime(hora, 100, "\nYo(Cliente) (%H:%M)->", tm);
		printf("%s", hora);
		gets(sendline);
		tmp=strcat(hora, sendline);
		fputs(tmp, myf);
		write(sock_fd, sendline, strlen(sendline)+1);//para enviar el mensaje
		if(!strstr(cadena, "Hasta luego")){//si el mensaje no es hasta luego, deberemos responder el mensaje
			strftime(hora, 100, "\nServidor(%H:%M)->", tm);
			read(sock_fd, cadena, 100);//llenamos la cadena
			tmp=strcat(hora, cadena);
			printf("%s", tmp);//imprimimos la cadena en la consola
			fputs(tmp, myf);//Para guardar nuestra conversacion en un archivo de texto con el apuntador file que creamos al inicio
		}
	}
	printf("\n\nConversacion finalizada\n");//imprimimos que la conversacion termino
	printf("\n\nSe genero el archivo -> Conversacion_cliente.txt\n"); 
	fclose(myf);//cerramos el archivo de texto con la funcion fclose
}
		
		
