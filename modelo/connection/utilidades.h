#ifndef UTILIDADES_H_INCLUDED
#define UTILIDADES_H_INCLUDED


#include <bitset>
#include "NoSuchMessageExpectedException.h"
#include "MySocket.h"
#include "ConstantesEstadoDelJuego.h"
#define TAMANIO_BIT_SET 17
using namespace std;

inline void respuesta_a_mensajeDeSiSigoVivo(MySocket* socket){
    char mensaje[3+1];
    memcpy(mensaje,FIRMA_DEL_PROTOCOLO,TAMANIO_FIRMA_DEL_PROTOCOLO);
    mensaje[2]=SI_SIGO_VIVO;
    mensaje[3]='\0';
    socket->write(mensaje,3);
    cout<<"Mensaje enviado:"<<mensaje<<endl;
}

inline void asignaNumero(bitset<TAMANIO_BIT_SET>& bits,unsigned short& numero,int desde,int hasta){
    bitset<16> bitsSalida;
    int i=0;
    while(desde<=hasta){
        bitsSalida.set(i++,bits.test(desde++));
    }
    numero=(unsigned short)bitsSalida.to_ulong();
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


inline void asignaBits(bitset<TAMANIO_BIT_SET>& bits,unsigned short num,int desde,int hasta){
    unsigned char prueba=1;//Numero minimo.
    while(desde<=hasta){
        bits.set(desde++,num&prueba);
        prueba<<=1;
    }
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

#endif // UTILIDADES_H_INCLUDED
