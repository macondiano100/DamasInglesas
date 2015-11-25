#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtConcurrent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialogoConecccion= new DialogoConeccion(this);
    dialogoOpciones=new DialogoOpciones(this);
    dialogoOpciones->setModal(Qt::WindowModal);
    progresoConnecion=new QProgressDialog(tr("Esperando connecciones"),tr("Cancelar"),0,0,this);
    progresoConnecion->setFixedSize(width()/2,progresoConnecion->height());
    controlador=new Controler(this,this->ui->visualBoard);
    ui->visualBoard->connectControler(controlador);
    connect(ui->action_nueva_partida,SIGNAL(triggered()),this,SLOT(iniciarPartida()));
    connect(progresoConnecion,&QProgressDialog::canceled,&MessagesSender::cancelarInicioPartida);
    connect(&futureWatcher,SIGNAL(finished()),progresoConnecion,SLOT(cancel()));
    connect(ui->actionOpciones,SIGNAL(triggered()),this,SLOT(lanzaVentanaOpciones()));
}
void MainWindow::lanzaVentanaOpciones()
{
    dialogoOpciones->setNameFieldContent(controlador->getNombreUsuario());
    dialogoOpciones->setPortFieldContent(controlador->getPuerto());
    dialogoOpciones->exec();
    if(dialogoOpciones->wasOkPressed())
    {
        controlador->setNombreUsuario(dialogoOpciones->getNameFieldContent());
        controlador->setPuerto(dialogoOpciones->getPortFieldContent());
    }
}

void MainWindow::closeEvent(QCloseEvent *)
{
    MessagesSender::closeConnection();
}


void MainWindow::unirseAPartida()
{
    dialogoConecccion->setNombreUsuario(controlador->getNombreUsuario());
    dialogoConecccion->exec();
    if(MessagesSender::hayConeccion())
    {
        inicializarNumeroFichas();
        controlador->iniciaJuegoComoOponente();
        repaint();
    }
}

void MainWindow::iniciarPartida()
{
    QFuture<void> future=QtConcurrent::run(&MessagesSender::iniciarPartida,controlador->getPuerto(),
                                           controlador->getNombreUsuario());
    futureWatcher.setFuture(future);
    progresoConnecion->setModal(Qt::WindowModal);
    progresoConnecion->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
    progresoConnecion->exec();
    futureWatcher.waitForFinished();
    if(MessagesSender::hayConeccion()){
        inicializarNumeroFichas();
        controlador->iniciaJuegoComoHost();
        repaint();
    }
}

void MainWindow::actualizarNumeroFichas()
{
    ui->labelNumeroFichasRojas->setText(QString::number(tablero.getPrimerJugador()->getFichasActuales()));
    ui->labelNumeroFichasBlancas->setText(QString::number(tablero.getSegundoJugador()->getFichasActuales()));
}

void MainWindow::inicializarNumeroFichas()
{
    ui->groupBOxJugadorRojas->setTitle(QString(tablero.getPrimerJugador()->getNombre().c_str()));
    ui->groupBoxJugadorBlancas->setTitle(QString(tablero.getSegundoJugador()->getNombre().c_str()));
    ui->labelNumeroFichasRojas->setText(QString::number(tablero.getPrimerJugador()->getFichasActuales()));
    ui->labelNumeroFichasBlancas->setText(QString::number(tablero.getSegundoJugador()->getFichasActuales()));
}
MainWindow::~MainWindow()
{
    delete ui;
    delete dialogoOpciones;
    delete dialogoConecccion;
}
