#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QMutex>
#include "modelo/Tablero.h"
#include "modelo/Ficha.h"
#include "modelo/connection/MySocket.h"
#include "modelo/connection/ConstantesEstadoDelJuego.h"
#include "modelo/connection/NoSuchMessageExpectedException.h"
#include "modelo/connection/utilidades.h"
extern Tablero tablero;
class MessagesSender {
    static bool esperarConecciones;
    static bool responderKeepAlive;
    static bool estaConectado;
    static constexpr int BUFFER=461;
    static  MySocket* socketServidor;
    static  MySocket* socketCliente;
    static void inicializa_tablero();

public:
    static void cancelarInicioPartida();
    static void enviarInformacionDeTurno(int nTurno,vector<Movimiento> &movements,bool partidaGanada);
    static void esperarTurnoOponente(uint32_t &nTurno, vector<Movimiento> &movements, uint8_t &banderas);
    static void enviarRespuestaDeTurno(u_int8_t banderas,int numeroTurno);
    static void esperaRespuestaDeTurno(u_int8_t &banderas,u_int32_t &numeroTurno);
    static void iniciarPartida(string puerto, string nombreUsuario);
    static void closeConnection();
    static void unirse_a_partida(string host, string puerto, string nombreAusar);
    static void responderKeepAlives();
    static void pararRespuestasKeepAlive();
    static bool hayConeccion();

};







#endif // CONNECTIONMANAGER_H
