#include <QHBoxLayout>
#include <QPainter>
#include <QColor>
#include <QString>
#include "visualboardsquare.h"
#include "modelo/Ficha.h"
#include <iostream>

VisualBoardSquare::VisualBoardSquare():VisualBoardSquare(0,0)
{
}
VisualBoardSquare::VisualBoardSquare(int fila, int columna):fila(fila),columna(columna)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

}

void VisualBoardSquare::paintEvent(QPaintEvent * e)
{
    Super::paintEvent(e);
    QPainter painter(this);
    QColor c;
    c.setRgb(255,255,255);
    painter.setPen(c);
    Ficha* f=tablero.consultar(fila,columna);
    if(f)
    {
        QImage sourceImage=
                f->getColor()==COLOR_BLANCO?
                    getIcono_ficha_blanca():getIcono_ficha_roja();
        sourceImage=sourceImage.scaled(painter.device()->width(),painter.device()->height(),Qt::KeepAspectRatio);
        QRect rect=sourceImage.rect();
        QRect devrect(0,0,painter.device()->width(),painter.device()->height());
        rect.moveCenter(devrect.center());
        painter.drawImage(rect.topLeft(),sourceImage);
        if(f->esDobleMovimiento())
        {
            QImage crownImage=
                    getIcono_corona().scaled(
                        painter.device()->width()/3,
                        painter.device()->height()/3,
                        Qt::KeepAspectRatio);
            rect=crownImage.rect();
            QRect devrect2(0,0,painter.device()->width(),painter.device()->height());
            rect.moveCenter(devrect2.center());
            painter.drawImage(rect.topLeft(),crownImage);
        }
    }
}

QImage &VisualBoardSquare::getIcono_ficha_roja()
{
    static QImage icono_ficha_roja=QImage(":/image/resources/piezaRoja.png");
    return icono_ficha_roja;
}

QImage &VisualBoardSquare::getIcono_ficha_blanca()
{
    static QImage icono_ficha_blanca=QImage(":/image/resources/piezaBlanca.png");
    return icono_ficha_blanca;
}

QImage &VisualBoardSquare::getIcono_corona()
{
    static QImage icono_corona=QImage(":/image/resources/crown.png");
    return icono_corona;
}

VisualBoardSquare::~VisualBoardSquare()
{

}

