#include <iostream>

using namespace std;

#include "Tablero.h"
#include "Ficha.h"
#include "MySocket.h"
#include "ConstantesEstadoDelJuego.h"
#include "NoSuchMessageExpectedException.h"
#include "utilidades.h"

#define BUFFER 461

MySocket* socketServidor;
MySocket* socketCliente;
Jugador* jug1;
Jugador* jug2;
Tablero* tablero;

enum opcionesMenu{
    INICIAR_PARTIDA,
    UNIRSE_A_PARTIDA,
    SALIR
};
int dameOpcionMenu();
void imprimeFichas();

void iniciarPartida();
void unirse_a_partida();
void inicializarPrograma();
unsigned int dameTipoDeMensaje(char* mensajeLeido);

void procesarTurno(char* mensaje,int n,MySocket* socket);

/*int main(){
    bool continuarPrograma=true;
    inicializarPrograma();
    do{
        switch(dameOpcionMenu()){
            case INICIAR_PARTIDA:iniciarPartida();break;
            case UNIRSE_A_PARTIDA:unirse_a_partida();break;
            case SALIR:continuarPrograma=false;break;
            default: cout<<"Opcion no valida\n";
        }
    }while(continuarPrograma);
    try{
        cout<<"Cerrando socket cliente"<<endl;
        socketCliente->close();
    }catch(SocketException* e){
    }
    try{
        if(socketServidor!=nullptr){
            cout<<"Cerrando socket servidor"<<endl;
            socketServidor->close();
        }
    }catch(SocketException* e){

    }
    delete jug1;
    delete jug2;
    delete tablero;
    if(socketServidor!=nullptr){
        delete socketServidor;
    }
    delete socketCliente;*/

    return 0;
}
void procesarTurno(char* mensaje,int n,MySocket* socket){
    char mensajeDeRespuesta[7+1];
    char firmaProtocolo[TAMANIO_FIRMA_DEL_PROTOCOLO+1];
    unsigned int numeroDeTurno;
    unsigned short movimientos,i;
    unsigned short filaOrigen,columnaOrigen,filaDest,columnaDest;
    bool esFichaComida;
    memcpy(firmaProtocolo,mensaje,TAMANIO_FIRMA_DEL_PROTOCOLO);
    firmaProtocolo[TAMANIO_FIRMA_DEL_PROTOCOLO]='\0';
    memcpy(&numeroDeTurno,&mensaje[TAMANIO_FIRMA_DEL_PROTOCOLO+1],4);
    numeroDeTurno=ntohs(numeroDeTurno);
    cout<<"Numero de turno:"<<numeroDeTurno<<endl;
    memcpy(&movimientos,&mensaje[TAMANIO_FIRMA_DEL_PROTOCOLO+5],2);
    movimientos=ntohs(movimientos);
    cout<<"Movimientos:" << movimientos<<endl;
    for(i=0;i<movimientos;i++){
        cout<<"Movimiento #"<< (movimientos+1)<<endl;
        fijaCoordenadas(mensaje,filaOrigen,columnaOrigen,filaDest,columnaDest,esFichaComida);
        cout<<"Fila orig:"<<filaOrigen<<endl;
        cout<<"Colum orig:"<<columnaOrigen<<endl;
        cout<<"Fila dest:"<<filaDest<<endl;
        cout<<"Colum dest:"<<columnaDest<<endl;
        cout<<"esFichaComida:"<<esFichaComida<<endl;
    }
    if(socket==socketServidor){
        numeroDeTurno++;
    }
    memcpy(mensajeDeRespuesta,firmaProtocolo,2);
    mensajeDeRespuesta[2]=htons(RESPUESTA_DE_TURNO);
    memcpy(&mensajeDeRespuesta[3],&numeroDeTurno,4);
    mensajeDeRespuesta[7+1]='\0';
    //cout<<"Mensaje enviado:"<<mensajeDeRespuesta<<endl;
    socket->write(mensajeDeRespuesta,7);
}
void imprimeFichas(){
    int i,j,n=tablero->getTamanio();
    Ficha* ficha;
    cout<<"Fichas actuales de jug1:"<< jug1->getFichasActuales()<<endl;
    cout<<"Fichas actuales de jug2:"<< jug2->getFichasActuales()<<endl;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            ficha=tablero->consultar(i,j);
            if(ficha!=nullptr){
                cout<<"color de ficha ["<<i<<"]["<<j<<"]="<<
                    (ficha->getColor()==COLOR_BLANCO?"blanco":"rojo")<<endl;
            }
        }
    }
}
int dameOpcionMenu(){
    int opc;
    cout<<"1.- Iniciar partida"<<endl;
    cout<<"2.- Unirse a partida"<<endl;
    cout<<"3.- Salir"<<endl;
    cout<<"Opcion:";
    cin>>opc;
    return opc-1;
}
void inicializarPrograma(){
    jug1=new Jugador(COLOR_ROJO);
    jug2=new Jugador(COLOR_BLANCO);
    tablero=new Tablero(jug1,jug2);
}
void unirse_a_partida(){
    jug2->setNombre("Diego Olvera");
    int i,j,l;
    char message[BUFFER+1];
    char mensaje_a_enviar[BUFFER+1];
    char firmaDelProtocolo[TAMANIO_FIRMA_DEL_PROTOCOLO+1];
    unsigned short bandera;
    char versionDelProtocolo;
    char nombreDelJugador[TAMANIO_NOMBRE_JUGADOR+1];
    string miNombreStr=jug2->getNombre();
    memcpy(mensaje_a_enviar,FIRMA_DEL_PROTOCOLO,2);
    for(i=2,l=0,j=miNombreStr.size();l<j;i++){
        mensaje_a_enviar[i]=miNombreStr[l++];
    }
    int tamanioNombre;
    int bytes;
    string host;
    cout<<"Host:";
    cin>>host;
    try{
        cout<<"Conectando..."<<endl;
        socketCliente=new MySocket(PUERTO,host);
        try{
            bytes=socketCliente->read(message,BUFFER);
            socketCliente->write(mensaje_a_enviar,i);
            memcpy(firmaDelProtocolo,message,TAMANIO_FIRMA_DEL_PROTOCOLO);
            firmaDelProtocolo[TAMANIO_FIRMA_DEL_PROTOCOLO]='\0';
            bandera=message[2];
            //bandera=ntohs(bandera);
            if(bandera!=INICIAR_PARTIDA || strcmp(firmaDelProtocolo,FIRMA_DEL_PROTOCOLO)!=0){
                cout<<"No se recibio la bandera adecuada"<<endl;
                socketCliente->shutdown();
                socketCliente->close();
                socketServidor->close();
                return;
            }
            versionDelProtocolo=message[3];
            memcpy(nombreDelJugador,&message[4],tamanioNombre=bytes-3);
            nombreDelJugador[tamanioNombre]='\0';
            cout<<"Firma del protocolo:"<<firmaDelProtocolo<<endl;
            cout<<"Bandera:"<<bandera<<endl;
            cout<<"Version del protocolo:"<<(unsigned short)versionDelProtocolo<<endl;
            cout<<"Nombre del jugador al que se ha conectado:"<<nombreDelJugador<<endl;
            jug1->setHost(socketCliente->getIp());
            cout<<"IP:"<< jug1->getHost()<<endl;
            jug1->setNombre(string(nombreDelJugador));

        }catch(SocketReadException& exRead){
            cout<<exRead.getMessage();
        }
    }catch(SocketException& e){
        cout<<e.getMessage()<<endl;
    }
}
void iniciarPartida(){
    jug1->setNombre("Jose Vidal");
    socketServidor=new MySocket(PUERTO);
    socketServidor->setFlags(O_NONBLOCK);
    socketServidor->listen(1);
    int i,j,indiceMensaje,bytes;
    string nombreJugador1;
    char firmaDelProtocolo[TAMANIO_FIRMA_DEL_PROTOCOLO+1];
    char nombreDelJugador[TAMANIO_NOMBRE_JUGADOR+1];
    char mensaje[BUFFER+1];
    char mensajeRecibido[BUFFER+1];
    int tamanioNombreJugador;
    cout<<"Esperando jugador..."<<endl;
    while( (socketCliente=socketServidor->accept())==nullptr );
    mensaje[0]=FIRMA_DEL_PROTOCOLO[0];
    mensaje[1]=FIRMA_DEL_PROTOCOLO[1];
    mensaje[2]=INICIAR_PARTIDA;
    mensaje[3]=VERSION_DEL_PROTOCOLO;
    nombreJugador1=jug1->getNombre();
    indiceMensaje=4;
    for(i=0,j=nombreJugador1.size();i<j;i++){
        mensaje[indiceMensaje++]=nombreJugador1.at(i);
    }
    try{
        socketCliente->write(mensaje,indiceMensaje);
        bytes=socketCliente->read(mensajeRecibido,BUFFER);
        memcpy(firmaDelProtocolo,mensajeRecibido,2);
        firmaDelProtocolo[TAMANIO_FIRMA_DEL_PROTOCOLO]='\0';
        memcpy(nombreDelJugador,&mensajeRecibido[2],tamanioNombreJugador=bytes-2);
        nombreDelJugador[tamanioNombreJugador]='\0';
        cout<<"Firma del protocolo:"<<firmaDelProtocolo<<endl;
        cout<<"Nombre del jugador 2:"<<nombreDelJugador<<endl;
        if(strcmp(firmaDelProtocolo,FIRMA_DEL_PROTOCOLO)!=0){
            cout<<"No se recibio la bandera adecuada"<<endl;
            socketCliente->shutdown();
            socketCliente->close();
            return;
        }
        jug2->setHost(socketCliente->getIp());
        cout<<"IP:"<< jug2->getHost()<<endl;
        jug2->setNombre(string(nombreDelJugador));
    }catch(SocketWriteException& ex){
        cout<<ex.getMessage()<<endl;
    }
}
