#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include "modelo/Tablero.h"
#include "modelo/Ficha.h"
#include "modelo/connection/MySocket.h"
#include "modelo/connection/ConstantesEstadoDelJuego.h"
#include "modelo/connection/NoSuchMessageExpectedException.h"
#include "modelo/connection/utilidades.h"
#include "modelo/connection/ConstantesEstadoDelJuego.h"
extern Tablero tablero;
class MessagesSender {
    static bool esperarConecciones;
    static constexpr int BUFFER=461;
    static MySocket* socketServidor;
    static MySocket* socketCliente;
    static void inicializa_tablero();

public:
    static void cancelarInicioPartida();

    static void iniciarPartida();
    static void unirse_a_partida(string host);

};
#endif // CONNECTIONMANAGER_H
