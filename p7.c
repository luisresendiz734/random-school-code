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

unsigned char MAC_ORIGIN[6],
MAC_BROAD[6]={0xff,0xff,0xff, 0xff,0xff,0xff},
ethertype[2]={0x08, 0x06},
IPorigen[4],
IPdestino[4],
trama[1600], 
recev[1600], 
tipoh[2]={00,01}, 
tipoP[2]={0x08,0x00}, 
lonH[1]={6}, 
longP[1]={4}, 
codO[2]={01};

int get_socket_data(int socket_descriptor);
void display(unsigned char buffer[], int type);
int send_trama(int socket_descriptor, int index);
int receive_trama(int socket_descriptor);
int display_received_trama(int tlen);

int main() {

	int socket_descriptor = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	if(socket_descriptor == -1) {
		printf("Open socket failed\n");
		return 1;
	}
	
	int index = get_socket_data(socket_descriptor);

	if(index == -1) return 1;

  memcpy(trama+0, MAC_BROAD, 6);
  memcpy(trama+6, MAC_ORIGIN, 6);
  memcpy(trama+12, ethertype, 2);
  memcpy(trama+14, tipoh, 2);
  memcpy(trama+16, tipoP, 2);
  memcpy(trama+18, lonH, 1);
  memcpy(trama+19, longP, 1);
  memcpy(trama+20, codO, 2);
  memcpy(trama+22, MAC_ORIGIN, 6);
  memcpy(trama+28, IPorigen, 4);
  memset(trama+32, 0x00, 6);
  memcpy(trama+38, IPdestino, 4);

  if(send_trama(socket_descriptor, index) || receive_trama(socket_descriptor)) return 1;

  close(socket_descriptor);

	return 0;
}

int get_socket_data(int socket_descriptor) {

	char interface_name[10];
	unsigned char buffer[6];
	int id = -1;

	struct ifreq socket_info;

	scanf("%s", interface_name);
	strcpy(socket_info.ifr_name, interface_name);
	
	if(ioctl(socket_descriptor, SIOCGIFINDEX, &socket_info) == -1) {
		printf("get socket index failed\n");
		return -1;
	}

	printf("socket index: %d\n", socket_info.ifr_ifindex);
	id = socket_info.ifr_ifindex;
	
	if(ioctl(socket_descriptor, SIOCGIFHWADDR, &socket_info) == -1) {
		printf("get mac address failed\n");
		return -1;
	}

	memcpy(MAC_ORIGIN, socket_info.ifr_hwaddr.sa_data + 0, 6);
	printf("MAC: ");
	display(MAC_ORIGIN, 1);

	if(ioctl(socket_descriptor, SIOCGIFADDR, &socket_info) == -1) {
		printf("get ip address failed\n");
		return 1;
	}

	memcpy(buffer, socket_info.ifr_addr.sa_data, 6);
	printf("IP: ");
	display(buffer, 2);

	if(ioctl(socket_descriptor, SIOCGIFNETMASK, &socket_info) == -1) {
		printf("get subnetwork mask failed\n");
		return 1;
	}

	memcpy(buffer, socket_info.ifr_netmask.sa_data, 6);
	printf("MASK: ");
	display(buffer, 2);

	if(ioctl(socket_descriptor, SIOCGIFBRDADDR, &socket_info) == -1) {
		printf("get broadcast address failed\n");
		return 1;
	}

	memcpy(buffer, socket_info.ifr_broadaddr.sa_data, 6);
	printf("BROADCAST: ");
	display(buffer, 2);

	return id;
}

void display(unsigned char buffer[], int type) {
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

int send_trama(int socket_descriptor, int index) {

  struct sockaddr_ll socket_interface;
  memset(&socket_interface, 0x00, sizeof(socket_interface));
  socket_interface.sll_family 	= AF_PACKET;
  socket_interface.sll_protocol = htons(ETH_P_ALL);
  socket_interface.sll_ifindex 	= index;

  int length = sendto(
  	socket_descriptor,
  	trama,
  	60,
  	0,
  	(struct sockaddr *) &socket_interface,
  	sizeof(socket_interface)
  );

  if(length == -1){
    printf("Send trama failed\n");
    return 1;
  }

  printf("Send trama success\n");
  return 0;
}

int receive_trama(int socket_descriptor) {
	int error_status = 0;
	while(666) {
    int tlen = recvfrom(socket_descriptor, recev, 60, 0, NULL, 0);
    if(tlen == -1){
      printf("Receive trama failed\n");
      error_status = 1;
      break;
    }
    
    if(!memcmp(recev+6,MAC_ORIGIN,6)||!memcmp(recev+0,MAC_ORIGIN,6));
    error_status = display_received_trama(tlen);
    break;
  }

  return error_status;
}

int display_received_trama(int tlen) {
	int i;
  printf("\n");
  printf("Destination address: ");
  for(i=0; i<6; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");
  printf("Source address: "); 
  for(i=6; i<12; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");

  printf("Ethertype: ");
  for(i=12; i<14; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");
  printf("Hardware type: ");
  for(i=14; i<16; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");
  printf("Protocol type: "); 
  for(i=16; i<18; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");
  printf("Hardware address length: ");
  for(i=18; i<19; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");
  printf("Protocol address length: "); 
  for(i=19; i<20; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");
  printf("Operation code: ");
  for(i=20; i<22; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");


  printf("Origin hardware address: ");
  for(i=22; i<28; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");
  printf("Origin protocol address: ");
  for(i=28; i<32; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");
  printf("Destination hardware address: ");
  for(i=32; i<38; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");
  printf("Destination protocol address: ");
  for(i=38; i<42; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");
  printf("Trama: ");
  for(i=42; i<tlen; i++){
    printf("%.2x ", recev[i]);
  }
  printf("\n");
  return 0;
}