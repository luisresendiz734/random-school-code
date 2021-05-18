/*
	Practica: 8
	Nombre: Enviar una trama
	Asignatura: Redes de computadoras
	Profesor: Fabian Gaspar Medina
	Fecha: 14/mayo/2021
	Equipo: 3
	Integrantes:
		- Perez Flores Emilie Constanza
		- Resendiz Chavez Luis Fernando
		- Solis Hernandez Viridiana
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <net/if.h>

// variable para darle un tamano a la trama a enviar
#define FRAME_SIZE 1024
// para no escribir todo el tiempo "unsigned char", mejor solo "unc"
#define unc unsigned char

unc MAC_ORIGIN[6]; // array para obtener la direccion mac de origen
unc MAC_DEST[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }; // array con la direccion mac de destino o broadcast
unc FRAME[FRAME_SIZE], FRAME_RECV[FRAME_SIZE], ethertype[2] = { 0x0c, 0x0c }; // trama y ethertype


// Prototipos de funciones

int get_socket_data(int socket_descriptor);
void create_frame();
int send_frame(int socket_descriptor, int index);
void display(unc buffer[], int type);
void imprimirTrama();
void recibirTrama(int socket_descriptor);

int main() {
	int socket_descriptor = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	if(socket_descriptor == -1) {
		printf("Open socket: error\n");
		return 1;
	}

	printf("Open socket: OK\n");

	int index = get_socket_data(socket_descriptor);

	create_frame();

	int send_status = send_frame(socket_descriptor, index);
	recibirTrama(socket_descriptor);

	close(socket_descriptor);

	return send_status;
}


/*
	Name: get_socket_data
	Params: {
		socket_descriptor: int
	}
	Return: (int) retorna el index obtenido
	Description: obtenemos la informacion necesaria para la trama en este caso index y MAC
*/
int get_socket_data(int socket_descriptor) {

	char interface_name[10];
	unsigned char buffer[6];
	int id = -1;

	struct ifreq socket_info;

	scanf("%s", interface_name);
	strcpy(socket_info.ifr_name, interface_name);
	
	if(ioctl(socket_descriptor, SIOCGIFINDEX, &socket_info) == -1) {
		printf("get socket index: error\n");
		return -1;
	}

	printf("socket index: %d\n", socket_info.ifr_ifindex);
	id = socket_info.ifr_ifindex;
	
	if(ioctl(socket_descriptor, SIOCGIFHWADDR, &socket_info) == -1) {
		printf("get mac address: error\n");
		return -1;
	}

	memcpy(MAC_ORIGIN, socket_info.ifr_hwaddr.sa_data + 0, 6);
	printf("MAC origin: ");
	display(MAC_ORIGIN, 1);

	return id;
}


/*
	Name: create_frame
	Params: {
		socket_descriptor: int,
		index: int
	}
	Return: void
	Description: crea la trama a enviar agregando un mensaje por parte del usuario (o nosotros).
*/
void create_frame() {
	memcpy(FRAME + 0, MAC_DEST, 6);
	memcpy(FRAME + 6, MAC_ORIGIN, 6);
	memcpy(FRAME + 12, ethertype, 2);
	memcpy(FRAME + 14, "Hola a todos", 33);
}


/*
	Name: send_frame
	Params: {
		socket_descriptor: int,
		index: int
	}
	Return: (int) si algo falla retorna 1, si todo sale bien retorna 0.
	Description: crea una nueva estructura y envia la trama
*/
int send_frame(int socket_descriptor, int index) {

  struct sockaddr_ll socket_interface;
  memset(&socket_interface, 0x00, sizeof(socket_interface));
  
  socket_interface.sll_family 	= AF_PACKET;
  socket_interface.sll_protocol = htons(ETH_P_ALL);
  socket_interface.sll_ifindex 	= index;

  int bites_sent = sendto(
  	socket_descriptor,
  	FRAME,
  	60,
  	0,
  	(struct sockaddr *) &socket_interface,
  	sizeof(socket_interface)
  );

  if(bites_sent == -1){
    printf("Send frame: failed\n");
    return 1;
  }

  printf("Send frame: OK\n");
  return 0;
}


/*
	Name: display
	Params: {
		buffer: unsigned char[],
		type: int
	}
	Return: void
	Description: imprime un arreglo dependiendo el tipo: 1 = %2x, 2 = %2d
*/
void display(unc buffer[], int type) {
	if(type == 1) {
		for(int i = 0; i < 6; ++i) {
			printf("%2x:", buffer[i]);
		}
	} else {
		for(int i = 2; i < 6; ++i) {	
			printf("%2d:", buffer[i]);
		}
	}
	printf("\n");
}

void imprimirTrama() {

    for(int i = 0; i < 16; i++) {
        printf("%.2x:",FRAME_RECV[i]);
    }
    printf("\n");
}


void recibirTrama(int socket_descriptor) {
    while(1) {
    	int tam = recvfrom(socket_descriptor,FRAME_RECV,FRAME_SIZE,0,NULL,0);
	    if(tam == -1) {
		    printf("Error al Recibir\n");
		    exit(0);
		} 
		
		imprimirTrama();
    }
 
}
