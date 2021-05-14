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

#define FRAME_SIZE 1024
#define unc unsigned char

unc MAC_ORIGIN[6];
unc MAC_DEST[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
unc FRAME[FRAME_SIZE], ethertype[2] = { 0x0c, 0x0c };

int get_socket_data(int socket_descriptor);
void create_frame();
int send_frame(int socket_descriptor, int index);
void display(unc buffer[], int type);

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

	close(socket_descriptor);

	return send_status;
}

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

void create_frame() {
	memcpy(FRAME + 0, MAC_DEST, 6);
	memcpy(FRAME + 6, MAC_ORIGIN, 6);
	memcpy(FRAME + 12, ethertype, 2);
	memcpy(FRAME + 14, "Equipo 3", 8);
}

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