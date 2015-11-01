#include "Dialogoconeccion.h"
#include "ui_Dialogoconeccion.h"

DialogoConeccion::DialogoConeccion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogoConeccion)
{
    ui->setupUi(this);
}
DialogoConeccion::~DialogoConeccion()
{
    delete ui;
}
