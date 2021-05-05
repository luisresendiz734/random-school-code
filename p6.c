#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <net/if.h>

int get_socket_data(int socket_descriptor);
void display(unsigned char buffer[], int type);

int main() {

	int socket_descriptor = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	if(socket_descriptor == -1) {
		printf("Open socket failed\n");
		return 1;
	}
	
	int process_status = get_socket_data(socket_descriptor);
	
	close(socket_descriptor);

	return process_status;
}

int get_socket_data(int socket_descriptor) {

	char interface_name[20];
	unsigned char buffer[6];

	struct ifreq socket_info;

	scanf("%s", interface_name);
	strcpy(socket_info.ifr_name, interface_name);
	
	if(ioctl(socket_descriptor, SIOCGIFINDEX, &socket_info) == -1) {
		printf("get socket index failed\n");
		return 1;
	}

	printf("socket index: %d\n", socket_info.ifr_ifindex);
	
	if(ioctl(socket_descriptor, SIOCGIFHWADDR, &socket_info) == -1) {
		printf("get mac address failed\n");
		return 1;
	}

	memcpy(buffer, socket_info.ifr_hwaddr.sa_data, 6);
	printf("MAC: ");
	display(buffer, 1);

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

	return 0;
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



























