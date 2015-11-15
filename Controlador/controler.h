#ifndef CONTROLER_H
#define CONTROLER_H
class MainWindow;
#include "visual/mainwindow.h"
#include "visual/visualboard.h"
#include "modelo/connection/utilidades.h"
namespace Ui {
class Controler;
}
class Controler:QObject
{
    MainWindow* mainWindow;
    VisualBoard* visualBoard;
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
    QFutureWatcher<void> futureWatcher;
    bool waitAndProcessAnswer(bool partidaGanada);
    void cierreForsozo();
    void seGanoLaPartida();
    bool waitAndProcessOponentMoves(u_int8_t &banderasRespuesta);
public slots:
void casillaClicked();
public:

    VisualBoardSquare* sourceSquare;
    Controler(MainWindow*,VisualBoard*);
};

#endif // CONTROLER_H
