#ifndef VISAULBOARD_H
#define VISAULBOARD_H

#include <QWidget>
#include<array>
#include<QGridLayout>
#include<QVariant>
#include<vector>
#include<QProgressDialog>
#include<QFutureWatcher>
#include<QFuture>
class Controler;
#include "Controlador/controler.h"
#include "modelo/connection/utilidades.h"
#include "visualboardsquare.h"
using std::array;
extern Tablero tablero;
class VisualBoard : public QWidget
{
    Q_OBJECT
private:
    enum Estado
    {
        WAITING_SOURCE,
        WAITING_DESTINY,
        WAITING_OPONENT
    };
    Estado estadoActual;
    static constexpr int BOARD_SIZE=8;
    array<array<VisualBoardSquare*,BOARD_SIZE>,BOARD_SIZE> squares;
    QGridLayout* gridLayout;
    bool forcedMove;
public:
    void connectControler(Controler *controler);
    void turnOffSquares();
    QProgressDialog* progressDialogWaitingOponent;
    explicit VisualBoard(QWidget *parent = 0,int inverted=false);
    void invertirTablero();
    void highLightSquares(bool forcedMove, VisualBoardSquare *sourceSquare);
    ResultadoDeMovimiento doMovements(vector<Movimiento> movimientos);
    ~VisualBoard();

signals:

public slots:
};

#endif // VISAULBOARD_H
