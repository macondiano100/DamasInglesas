#ifndef VISUALBOARDSQUARE_H
#define VISUALBOARDSQUARE_H
#include <QPushButton>
#include <QImage>
#include <QVariant>
#include "modelo/Tablero.h"
extern Tablero tablero;
class VisualBoardSquare : public QPushButton
{
    Q_OBJECT
public:
    VisualBoardSquare();
    VisualBoardSquare(int fila,int columna);
    void setHighlight(bool highlighted)
    {
        setProperty("highlight",highlighted);
        setStyleSheet(styleSheet());
    }
    virtual ~VisualBoardSquare();
    typedef QPushButton Super;
    int fila;
    int columna;
protected:
    void paintEvent(QPaintEvent *);
private:
    static QImage &getIcono_ficha_roja();
    static QImage &getIcono_ficha_blanca();
    static QImage &getIcono_corona();
signals:
public slots:
};

#endif // VISUALBOARDSQUARE_H
