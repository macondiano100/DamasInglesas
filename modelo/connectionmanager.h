#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>

#include "modelo/Tablero.h"
#include "modelo/Ficha.h"
#include "modelo/connection/MySocket.h"
#include "modelo/connection/ConstantesEstadoDelJuego.h"
#include "modelo/connection/NoSuchMessageExpectedException.h"
#include "modelo/connection/utilidades.h"

#define BUFFER 461
extern Tablero tablero;
MySocket* socketServidor;
MySocket* socketCliente;
namespace connection {
bool esperarConecciones;
void cancelarInicioPartida()
{
    esperarConecciones=false;
}

void iniciarPartida(){
    Jugador *jug1,*jug2;
    int *aleph=new int();
    delete aleph;
    jug1=new Jugador();
    jug2=new Jugador();
    jug1->setNombre("Jose Vidal");
    if(socketServidor)
    {
        cout<<"endl"<<endl;
        socketServidor->close();
        delete socketServidor;
    }
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
    int a=0;
    esperarConecciones=true;
    while( (socketCliente=socketServidor->accept())==nullptr )
    {
        if(esperarConecciones)cout<<a++<<endl;
        else return;
    }
    mensaje[0]=FIRMA_DEL_PROTOCOLO[0];
    mensaje[1]=FIRMA_DEL_PROTOCOLO[1];
    mensaje[2]=INICIO_PARTIDA;
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

}

#endif // CONNECTIONMANAGER_H
