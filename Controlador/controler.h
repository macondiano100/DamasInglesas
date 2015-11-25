#ifndef CONTROLER_H
#define CONTROLER_H
class MainWindow;
#include "visual/mainwindow.h"
class VisualBoard;
#include "visual/visualboard.h"
class VisualBoardSquare;
#include "visual/visualboardsquare.h"
#include "modelo/connection/utilidades.h"
class Controler:public QObject
{
    Q_OBJECT
    MainWindow* mainWindow;
    VisualBoard* visualBoard;
    VisualBoardSquare* sourceSquare;
    enum Estado
    {
        WAITING_SOURCE,
        WAITING_DESTINY,
        WAITING_OPONENT
    };
    Estado estadoActual;
    vector<Movimiento> lastMovements;
    bool forcedMove;
    u_int32_t NTURNO;
    bool waitAndProcessAnswer(bool partidaGanada);
    void cierreForsozo();
    void seGanoLaPartida(Jugador *ganador);
    string nombreJugador;
    string puerto;
    function<Jugador*()> getJugadorUsuario;
    function<Jugador*()> getJugadorOponente;
public slots:
void casillaClicked();
public:
    bool waitAndProcessOponentMoves(u_int8_t &banderasRespuesta);
    void iniciaJuegoComoHost();
    void iniciaJuegoComoOponente();
    string getNombreUsuario();
    string getPuerto();
    void setNombreUsuario(string);
    void setPuerto(string);
    QFutureWatcher<void> futureWatcherEspearaOponente;
    Controler(MainWindow*,VisualBoard*);

};

#endif // CONTROLER_H
