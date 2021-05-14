#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>


//VARIABLES GLOBALES NECESAIRAS

//DIRECCION MAC DE ORIGEN
unsigned char MACorigen[6];
//DIRECCION MAC DE DESTINO, EN ESTE CASO FF FF FF FF FF
unsigned char MACbroad[6]={0xff,0xff,0xff,0xff,0xff,0xff};
//TIPO DE PROTOCOLO UTILIZADO
unsigned char ethertype[2]={0x0c,0x0c};
//TRAMA ENVIADA.
unsigned char tramaEnv[1514];



//ESTRUCTURA DE LAS FUNCIONES DEL CODIGO


//FUNCION OBTENER DATOS, RECIBE UN ENTERO
int obtenerDatos (int ds){
    //ESTRUCTURA QUE DESCRIBE LA INTERFAZ DEL SOCKET
    struct ifreq nic;
    //ASIGNACION DE LA INTERFAZ
    char nombre[20];
    //VARIABLES PARA EL INDEX Y UN CONTADOR
    int i,index;
    //SOLICITAMOS LA INTERFAZ
    printf("\nInserta el nombre de la interfaz: ");
    //LEEMOS LA INTERFAZ
    scanf("%s", nombre);
    //COPIAMOS EL NOMBRE HACIA LA ESTRUCTURA IFREQ 
    strcpy(nic.ifr_name,nombre);
    //COMPROBAMOS SI LA DIRECCION ES IGUAL A LA DESCRITA EN NUESTRO HARDWARE
    if(ioctl(ds,SIOCGIFINDEX,&nic)==-1){
        perror("\n Error al obtener el index");
        exit(0);
    }else{
        //SI LA COMPROBACION ES CORRECTA, TOMAMOS EL VALOR DE INDEX DE NUESTRO STRUCT
        index=nic.ifr_ifindex;
        //IMPRIMIMOS EL INDICE
        printf("\n El indice es: %d", index); 
            //VERIFICAMOS NUESTRA DIRECCION MAC CON LA DESCRITA EN EL HARDWARE
            if(ioctl(ds,SIOCGIFHWADDR,&nic)==-1){
                perror("\nError al obtener la MAC");
                exit(0);
            }else{
                //COPIAMOS NUESTRA DIRECCION MAC, COMO MAC DE ORIGEN
                memcpy(MACorigen, nic.ifr_hwaddr.sa_data,6);
                //LA IMPRIMIMOS CON UN CICLO FOR.
                printf("\n La MAC es: ");
                for(i=0;i<6;i++){
                    printf("\n%.2x: ", MACorigen[i]);
                }
            }
    }
    //REGRESAMOS (RETORNAMOS) NUESTRO INDEX
    return index;
}

//ESTABLECEMOS UNA FUNCION QUE VAYA ASIGNANDO EL VALOR NECESARIO
//SIMULANDO UNA TRAMA DE RED
void estructuraTrama(unsigned char *trama){
    //DESTINO
    memcpy(trama+0,MACbroad,6);
    //ORIGEN
    memcpy(trama+6,MACorigen,6);
    //TIPO DE PROTOCOLO
    memcpy(trama+12,ethertype,2);
    //DATOS A ENVIAR
    memcpy(trama+14,"Practica 7 envia una Trama", 28);
}

//FUNCION ENVIAR TRAMA, ESTE RECIBE EL DESTINO, EL ORIGEN Y LA TRAMA A ENVIAR
void EnviarTrama (int ds, int index, unsigned char *trama){
    //UN ENTERO DEL TAMAÑO
    int tam;
    //OBTENEMOS LA PUERTA DE ENLACE DE NUESTRA INTERFAZ
    struct sockaddr_ll interfaz;
    //COPIAMOS LA INTERFAZ
    memset(&interfaz,0x00, sizeof(interfaz));
    //ASIGNAMOS A NUESTRO STRUCT LOS ELEMENTOS NECESARIOS
    interfaz.sll_family=AF_PACKET;
    interfaz.sll_protocol=htons(ETH_P_ALL);
    interfaz.sll_ifindex=index;
    //EL TAMAÑO A ENVIAR CONSIDERANDO EL SOCKET CREANDO CON ANTERIORIDAD
    tam=sendto(ds,trama,60,0,(struct sockaddr *)&interfaz, sizeof(interfaz));
    //SI ES SOLO 1, MANDA ERROR, EN OTRO CASO MANDA EL MENSAJE
    if(tam==1){
        perror("\nError al enviar");
        exit(0);
    }else perror("\nExito al enviar");
}

int main(){
    //ASIGNAMOS UN ENTERO AL SOCKET Y EL INDICE
    int packet_socket, indice;
    //ABRIMOS NUESTRO SOCKET.
    packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    //VERIFICAMOS QUE EL SOCKET HAYA SIDO ABIERTO
    if(packet_socket == -1){
        perror("\nError al abrir el socket");
        exit(0);
    //SI ABRIMOS EL SOCKET
    }else{
        //MANDAMOS A CONSOLA UNA SEÑAL
        perror("\nExito al abrir el socket");
        //CON LA FUNCION ANTERIOR OBTENEMOS EL INDICE
        indice=obtenerDatos(packet_socket);
        //ESTRUCTURAMOS LA TRAMA
        estructuraTrama(tramaEnv);
        //LA ENVIAMOS CON LA FUNCION ENVIAR TRAMA
        EnviarTrama(packet_socket,indice,tramaEnv);
    }
    //CERRAMOS EL SOCKET
    close(packet_socket);
    //CONCLUIMOS EL SOCKET
    return 0;
}
