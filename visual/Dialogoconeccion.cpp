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
    connect(ui->pushButton,&QPushButton::clicked,this,&QDialog::close);
    nombreUsuario="Diego Olvera";
}

void DialogoConeccion::setNombreUsuario(string nombre)
{
    nombreUsuario=nombre;
}
void DialogoConeccion::unirse_a_partida(){
    MessagesSender::unirse_a_partida(ui->addresField->text().toStdString(),
                                     ui->portField->text().toStdString(),
                                     nombreUsuario);
    close();
}

DialogoConeccion::~DialogoConeccion()
{
    delete ui;
}
