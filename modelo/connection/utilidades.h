#ifndef UTILIDADES_H_INCLUDED
#define UTILIDADES_H_INCLUDED

#define TAMANIO_BIT_SET 17
using namespace std;

#include <bitset>
#include <vector>
#include <iterator>
#include "NoSuchMessageExpectedException.h"
#include "MySocket.h"
#include "ConstantesEstadoDelJuego.h"

int fijaMensajeDeMovimiento(char* mensaje,unsigned short filaOrig,unsigned short columnaOrig,
    unsigned short filaDest,unsigned short columnaDest,bool esFichaComida);
void fijaCoordenadas(char* mensaje,unsigned short& filaOrig,unsigned short& columnaOrig,
    unsigned short& filaDest,unsigned short& columnaDest,bool& esFichaComida);
void asignaBits(bitset<TAMANIO_BIT_SET>& bit,unsigned short num,int desde,int hasta);
void asignaNumero(bitset<TAMANIO_BIT_SET>& bits,unsigned short& numero,int desde,int hasta);
void procesarMensaje(char* mensaje,int n,MySocket* socket)throw(NoSuchMessageExpectedException);
void procesarTurno(char* mensaje,int n,MySocket* socket);
void respuesta_a_mensajeDeSiSigoVivo(MySocket* socket);
unsigned int dameTipoDeMensaje(char* mensajeLeido);

inline void respuesta_a_mensajeDeSiSigoVivo(MySocket* socket){
    char mensaje[3+1];
    memcpy(mensaje,FIRMA_DEL_PROTOCOLO,TAMANIO_FIRMA_DEL_PROTOCOLO);
    mensaje[2]=SI_SIGO_VIVO;
    mensaje[3]='\0';
    socket->write(mensaje,3);
    //cout<<"Mensaje enviado:"<<mensaje<<endl;
}

inline void fijaCoordenadas(char* mensaje,unsigned short& filaOrig,unsigned short& columnaOrig,
    unsigned short& filaDest,unsigned short& columnaDest,bool& esFichaComida){
    unsigned int msnNumero;
    memcpy(&msnNumero,mensaje,3);
    bitset<TAMANIO_BIT_SET> bits(msnNumero);
    asignaNumero(bits,filaOrig,0,3);
    asignaNumero(bits,columnaOrig,4,7);
    asignaNumero(bits,filaDest,8,11);
    asignaNumero(bits,columnaDest,12,15);
    esFichaComida=bits.test(16);
}

inline void asignaNumero(bitset<TAMANIO_BIT_SET>& bits,unsigned short& numero,int desde,int hasta){
    bitset<16> bitsSalida;
    int i=0;
    while(desde<=hasta){
        bitsSalida.set(i++,bits.test(desde++));
    }
    numero=(unsigned short)bitsSalida.to_ulong();
}

inline void asignaBits(bitset<TAMANIO_BIT_SET>& bits,unsigned short num,int desde,int hasta){
    unsigned char prueba=1;//Numero minimo.
    while(desde<=hasta){
        bits.set(desde++,num&prueba);
        prueba<<=1;
    }
}
struct Movimiento
{
    unsigned short filaOrigen;
    unsigned short  colOrigen;
    unsigned short  filaDestino;
    unsigned short  colDestino;
    bool  fichaComida;
    Movimiento(unsigned short filaOrigen,unsigned short colOrigen,unsigned short filaDestino,
               unsigned short colDestino,bool fichaComida):
        filaOrigen(filaOrigen),colOrigen(colOrigen),filaDestino(filaDestino),colDestino(colDestino),
    fichaComida(fichaComida){}
    Movimiento():Movimiento(0,0,0,0,false){;}
};
inline vector<char> creaMensajeDeTurno(uint32_t numeroTurno,const vector<Movimiento>& movs)
{
    vector<char> mensaje;
    mensaje.insert(mensaje.end(),begin(FIRMA_DEL_PROTOCOLO),end(FIRMA_DEL_PROTOCOLO));
    mensaje.push_back(TURNO);//mensajeDeTurno
    numeroTurno=htonl(numeroTurno);
    mensaje.push_back(numeroTurno&(0xff));
    mensaje.push_back((numeroTurno>>8)&(0xff));
    mensaje.push_back((numeroTurno>>16)&(0xff));
    mensaje.push_back((numeroTurno>>24)&(0xff));//numero de turno
    uint16_t nTurnos;
    nTurnos=htons(movs.size());
    mensaje.push_back(nTurnos&(0xff));
    mensaje.push_back((nTurnos>>8)&(0xff));//cantidadDeTurnos
    char mensajeDeTurno[3]={};
    for(auto mov:movs)
    {
        fijaMensajeDeMovimiento(mensajeDeTurno,mov.filaOrigen,mov.colOrigen,
                                mov.filaDestino,mov.colDestino,mov.fichaComida);
        mensaje.insert(mensaje.end(),begin(mensajeDeTurno),end(mensajeDeTurno));
    }
    return mensaje;
}

inline int fijaMensajeDeMovimiento(char* mensaje,unsigned short filaOrig,unsigned short columnaOrig,
    unsigned short filaDest,unsigned short columnaDest,bool esFichaComida){
    unsigned int msn;
    bitset<TAMANIO_BIT_SET> bits;
    asignaBits(bits,filaOrig,0,3);
    asignaBits(bits,columnaOrig,4,7);
    asignaBits(bits,filaDest,8,11);
    asignaBits(bits,columnaDest,12,15);
    bits.set(16,esFichaComida);
    msn=(unsigned int)bits.to_ulong();
    memcpy(mensaje,&msn,3);
    return 10;
}
inline vector<Movimiento> getMovementsFromMessage(char* mensaje)
{
        int numeroDeTurno;
        unsigned short nMovimientos,i;
        vector<Movimiento> movimientos;
        Movimiento movimiento;
        memcpy(&numeroDeTurno,&mensaje[3],4);
        numeroDeTurno=ntohl(numeroDeTurno);
        cout<<"Numero de turno:"<<numeroDeTurno<<endl;
        memcpy(&nMovimientos,&mensaje[TAMANIO_FIRMA_DEL_PROTOCOLO+5],2);
        nMovimientos=ntohs(nMovimientos);
        cout<<"Movimientos:" << nMovimientos<<endl;
        for(i=0;i<nMovimientos;i++){
            cout<<"Movimiento #"<< (i+1)<<endl;
            fijaCoordenadas(&mensaje[(9+i*3)],movimiento.filaOrigen,movimiento.colOrigen,
                    movimiento.filaDestino,movimiento.colDestino,movimiento.fichaComida);
            movimientos.push_back(movimiento);
        }
        return movimientos;
}

#endif // UTILIDADES_H_INCLUDED
