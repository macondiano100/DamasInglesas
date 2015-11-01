#ifndef VISAULBOARD_H
#define VISAULBOARD_H

#include <QWidget>
#include<array>
#include<QGridLayout>
#include<QVariant>
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
        WAITING_DESTINY
    };
    Estado estadoActual;
    static constexpr int BOARD_SIZE=8;
    array<array<VisualBoardSquare*,BOARD_SIZE>,BOARD_SIZE> squares;
    QGridLayout* gridLayout;
    VisualBoardSquare* sourceSquare;
    void turnOffSquares();
    bool forcedMove;
private slots:
    void manejarMovimiento();
public:
    explicit VisualBoard(QWidget *parent = 0,int inverted=false);
    void invertirTablero();
    void highLightSquares();
    ~VisualBoard();

signals:

public slots:
};

#endif // VISAULBOARD_H
