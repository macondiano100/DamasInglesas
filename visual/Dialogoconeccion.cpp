#include <functional>
#include "modelo/connectionmanager.h"
#include "Dialogoconeccion.h"
#include "ui_Dialogoconeccion.h"
DialogoConeccion::DialogoConeccion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogoConeccion)
{
    ui->setupUi(this);
    auto unirse_a_partida=bind(MessagesSender::unirse_a_partida,ui->addresField->text().toStdString());
    connect(ui->boton_conectar,&QPushButton::clicked,unirse_a_partida);
}
DialogoConeccion::~DialogoConeccion()
{
    delete ui;
}
