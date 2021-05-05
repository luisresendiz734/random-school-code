//Librerias
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
//Declaracion de variables necesarias para la estructura de nuestra trama ARP
unsigned char MACorigen[6];
unsigned char MACbroad[6]={0xff,0xff,0xff, 0xff,0xff,0xff};
//unsigned char ethertype[2]={0x0c, 0x0c};
//solicitud de ARP
unsigned char ethertype[2]={0x08, 0x06};
unsigned char tramaEnv[1514], tramaRec[1514];
unsigned char IPorigen[4];
unsigned char IPdestino[4];
//Funcion imprimir trama, la cual nos mostrara los datos de nuestra trama
void imprimirTrama(unsigned char *paq, int len){ //funcion imprimirTrama
  int i; //variable contador
  printf("\n");//imprime salto de linea en pantalla
  printf("La direccion de destino es:");//imprime La direccion de destino es: en pantalla 
  ////ciclos for para imprimir las diferentes partes de la trama
  for(i=0; i<6; i++){ // for de 0 a 5
    printf("%.2x ", paq[i]);//imprime partede la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla
  printf("La direccion fuente es:");//imprime La direccion fuente es:  ,en pantalla 
  for(i=6; i<12; i++){// for de 6 a 11
    printf("%.2x ", paq[i]);//imprime parte de la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla

  printf("Ethertype:");//imprime Ethertype: ,en pantalla 
  for(i=12; i<14; i++){// for de 12 a 13
    printf("%.2x ", paq[i]);//imprime parte de la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla
  printf("Tipo de hardware:");//imprime Tipo de hardware: ,en pantalla 
  for(i=14; i<16; i++){// for de 14 a 15
    printf("%.2x ", paq[i]);//imprime parte de la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla
  printf("Tipo de protocolo:");//imprime Tipo de protocolo: ,en pantalla 
  for(i=16; i<18; i++){// for de 16 a 17
    printf("%.2x ", paq[i]);//imprime parte de la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla
  printf("Longitud de la direccion del hardware:");//imprime Longitud de la direccion del hardware: ,en pantalla 
  for(i=18; i<19; i++){// for de 18
    printf("%.2x ", paq[i]);//imprime parte de la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla
  printf("Longitud de la direccion de protocolos:");//imprime Longitud de la direccion de protocolos: ,en pantalla 
  for(i=19; i<20; i++){// for de 19
    printf("%.2x ", paq[i]);//imprime parte de la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla
  printf("Codigo de operacion:");//imprime Codigo de operacion: ,en pantalla 
  for(i=20; i<22; i++){// for de 20 a 21
    printf("%.2x ", paq[i]);//imprime parte de la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla


  printf("Direcciones de hardware del origen:");//imprime Direcciones de hardware del origen: ,en pantalla 
  for(i=22; i<28; i++){// for de 22 a 27
    printf("%.2x ", paq[i]);//imprime parte de la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla
  printf("Direcciones de protocolo de origen:");//imprime Direcciones de protocolo de origen: ,en pantalla 
  for(i=28; i<32; i++){// for de 28 a 31
    printf("%.2x ", paq[i]);//imprime parte de la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla
  printf("Direcciones de HW de destino:");//imprime Direcciones de HW de destino: ,en pantalla 
  for(i=32; i<38; i++){// for de32 a 37
    printf("%.2x ", paq[i]);//imprime parte de la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla
  printf("Direcciones de protocolo de origen:");//imprime Direcciones de protocolo de origen: ,en pantalla 
  for(i=38; i<42; i++){// for de 38 a 41
    printf("%.2x ", paq[i]);//imprime parte de la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla
  printf("Resto de la trama:");//imprime Resto de la trama: ,en pantalla 
  for(i=42; i<len; i++){// for de 42 a el tam de la trama
    printf("%.2x ", paq[i]);//imprime parte de la trama en el arreglo
  }
  printf("\n");//imprime salto de linea en pantalla
}
//Funcion estructuraTrama, la cual introducira los datos de la trama en nuestro arreglo de trama
void estructuraTrama(unsigned char *trama){ //funcion estructuraTrama
//Declaracion de variables unsigned char
  unsigned char tipoh[2]={00,01};//variable tipoh
  unsigned char tipoP[2]={0x08,0x00};//variable tipoP
  unsigned char lonH[1]={6};//variable lonH

  unsigned char longP[1]={4};//variable longP
  unsigned char codO[2]={01};//variable codO
  //encabezado MAC
  memcpy(trama+0, MACbroad, 6); //copia MAC destino en la trama
  memcpy(trama+6, MACorigen, 6);//copia MAC origen en la trama
  memcpy(trama+12, ethertype,2);//copia ethertype en la trama
  //Mesaje de ARP
  memcpy(trama+14,tipoh,2);//copia tipoh en la trama
  memcpy(trama+16,tipoP,2);//copia tipoP en la trama
  memcpy(trama+18,lonH,1);//copia lonH en la trama
  memcpy(trama+19,longP,1);//copia longP en la trama
  memcpy(trama+20,codO,2);//copia codO en la trama
  memcpy(trama+22, MACorigen, 6);//copia MACorigen en la trama
  memcpy(trama+28, IPorigen, 4);//copia IPorigen en la trama
  memset(trama+32, 0x00, 6);//copia 0x00 en la trama
  memcpy(trama+38,IPdestino, 4);//copia IPdestino en la trama
}
//funcion enviarTrama, se encarga de mandar nuestra trama
void enviarTrama( int ds, int index, unsigned char *trama){//funcion enviarTrama
//llenar nuestra estrucutra sockaddr
  int tam; //declaracion de variable tam entera
  struct sockaddr_ll interfaz; //declaracion de estructura interfaz tipo sockaddr_ll
  memset(&interfaz, 0x00, sizeof(interfaz));//copia el valor de interfaz
  interfaz.sll_family=AF_PACKET;//se asigna el valor de direccion ipV4
  interfaz.sll_protocol=htons(ETH_P_ALL);//se asigna el protocolo que se usara
  interfaz.sll_ifindex=index;//se asigna el valor del index
  //se hace el envio
  tam=sendto(ds, trama, 60, 0, (struct sockaddr *)&interfaz, sizeof(interfaz));//se hace el envio de la trama
  if(tam==-1){//si tam es igual a -1, hay un error 
    perror("\nError al enviar");//imprime Error al enviar, en pantalla
    exit(0);//termina el programa
  } else{ //si no se cumple la condicion 
    perror("\nexito al enviar");//imprime \nexito al enviar, en pantalla
  }
}
//funcion recibirTrama, encargada de recepcionar una trama

void recibirTrama(int ds, unsigned char *trama){ //funcion recibirTrama
//funcion para chat, recvfrom
  while(1){//ciclo si 1
    int tam;  //declaracion varible tam 
    tam=recvfrom(ds, trama, 60, 0, NULL, 0);//se hace la recepcion de la trama 
    if(tam==-1){//si tam es igual a -1, hay un error 
      perror("\nError al recibir");//imprime \nError al recibir, en pantalla
    }
    else{ //si no se cumple la condicion 
      if(!memcmp(trama+6,MACorigen,6)||!memcmp(trama+0,MACorigen,6));//compara la MACorigen con la de nuestra trama de desino y origen
      imprimirTrama(trama, tam);//imprime la trama 
      break;//sale del while
    }
  }  
}
//funcion obtenerDatos, encargada de sacar los datos para la trama 
int obtenerDatos(int ds){//funion obtenerDatos
  struct ifreq nic;//declaracion de nic tipo estructura ifreq
  unsigned char nombre[20]; //declaracion de nombre tipo unsigned char
  int i, index; // declaracion de i e index tipo entero 
  printf("Inserta el nombre de la interfaz: ");//imprime Inserta el nombre de la interfaz:,en pantalla
  gets(nombre);//solicitamos al usuario el nombre de nuestra interfaz de red
  strcpy(nic.ifr_name, nombre);//copiamos el nombre de nuestra interfaz de red en nic.ifr_name
  if(ioctl(ds, SIOCGIFINDEX, &nic)==-1){//obtenemos el index
    perror("\nerror al obtener el index");//imprime \nerror al obtener el index,en pantalla
  } else { 
    index=nic.ifr_ifindex;//asignamos el valor obtenido a index
    printf("El indice es %d", index);//imprime El indice es %d,en pantalla
    if(ioctl(ds, SIOCGIFHWADDR, &nic)==-1){//obtenemos la MAC
      perror("\nErro al obtener la MAC");//imprime \nErro al obtener la MAC,en pantalla
      exit(0);//termina el programa
    } else{ //en caso de que no exista error
      memcpy(MACorigen, nic.ifr_hwaddr.sa_data+0, 6);//copiamos la mac en la MACorigen 

      printf("\nLa MAC es: ");//imprime \nLa MAC es:,en pantalla
      for(i=0; i<6; i++){//for de 0 a 5
        printf("%.2x:", MACorigen[i]);//imprime %.2x:,en pantalla
      }
      printf("\nHOLA MUNDO");//imprime \nHOLA MUNDO,en pantalla
    /*printf("\nLa ip de origen es: ");
    for(i=0;i<6; i++){
      printf("%.2x:", MACorigen[i]);
    }*/
    }
  }
  return index;//regresa el index
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><>*/
int main(){//funcion main de nuestro programa
  int packet_socket, indice; //declaracion de indice y un socket 
  packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); //abrimos nuestro socket
  if(packet_socket==-1){//si el socket no se abre, error
    perror("Error al abrir el socket");//imprime Error al abrir el socket ,en pantalla
    exit(0);//termina el programa
  } else{ //en caso de que no exista error
    perror("\nExito al abrir el socket\n");//imprime \nExito al abrir el socket\n, en pantalla
    indice=obtenerDatos(packet_socket);//se le manda el descriptor de socket
    estructuraTrama(tramaEnv);//llamada a la funcion estructuraTrama
    enviarTrama(packet_socket, indice, tramaEnv);//llamada a la funcion enviarTrama
    recibirTrama(packet_socket, tramaRec);//llamada a la funcion recibirTrama
  }
  close(packet_socket); //cerramos nuestro socket
  return 0;//retorna 0 y acaba nuestra funcion main
}