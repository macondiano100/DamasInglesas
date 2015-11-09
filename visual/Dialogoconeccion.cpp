#include <functional>
#include "modelo/connectionmanager.h"
#include "Dialogoconeccion.h"
#include "ui_Dialogoconeccion.h"
#include<iostream>
DialogoConeccion::DialogoConeccion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogoConeccion)
{
    ui->setupUi(this);
    connect(ui->boton_conectar,SIGNAL(clicked()),this,SLOT(unirse_a_partida()));
}
void DialogoConeccion::unirse_a_partida(){
    MessagesSender::unirse_a_partida(ui->addresField->text().toStdString());
}

DialogoConeccion::~DialogoConeccion()
{
    delete ui;
}
