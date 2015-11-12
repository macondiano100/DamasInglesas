#include "connectionmanager.h"

bool MessagesSender::esperarConecciones;
MySocket* MessagesSender::socketServidor=nullptr;
MySocket* MessagesSender::socketCliente=nullptr;
#include <QMutex>
template<typename T, std::size_t N>
constexpr std::size_t size(T(&)[N]) { return N; } //tamaño de arreglo
constexpr int MAX_DATAGRAM_SIZE=1055;
void MessagesSender::inicializa_tablero()
{
    Jugador *jug1,*jug2;
    jug1=tablero.getPrimerJugador();
    jug2=tablero.getSegundoJugador();
    if(jug1) delete jug1;
    if(jug2) delete jug2;
    Tablero a;
    tablero=a;
}

void MessagesSender::cancelarInicioPartida()
{
    QMutex mutex;
    mutex.lock();
    esperarConecciones=false;
    mutex.unlock();
}
void MessagesSender::enviarInformacionDeTurno(int nTurno, vector<Movimiento> &movements,bool partidaGanada)
{
    vector<char> message=creaMensajeDeTurno(nTurno,movements);
    if(socketCliente!=nullptr)
    {
        socketCliente->write(message.data(),message.size());
    }
    else
    {
        for(auto c:message) cout<<(int)c<<endl;
    }
}
void MessagesSender::esperarTurnoOponente(uint32_t &nTurno, vector<Movimiento> &movements, uint8_t &banderas)
{
    char buffer[MAX_DATAGRAM_SIZE];
    if(socketCliente!=nullptr)
    {
        while(socketCliente->read(buffer,MAX_DATAGRAM_SIZE)<=0)
        {
            //TODO keepAlive!!;
        }
        memcpy(&banderas,&buffer[2],1);
        memcpy(&nTurno,&buffer[3],4);
        movements=getMovementsFromMessage(buffer);
        cout<<movements.size()<<endl;
        cout<<"Finished?"<<endl;
    }
    else
    {
        sleep(1);
    }
}


void MessagesSender::unirse_a_partida(string host){
    Jugador *jug1,*jug2;
    jug1=new Jugador();
    jug2=new Jugador();
    jug2->setNombre("Diego Olvera");
    int i,j,l;
    char message[BUFFER+1];
    char mensaje_a_enviar[BUFFER+1];
    char firmaDelProtocolo[TAMANIO_FIRMA_DEL_PROTOCOLO+1];
    unsigned short bandera;
    char versionDelProtocolo;
    char nombreDelJugador[TAMANIO_NOMBRE_JUGADOR+1];
    string miNombreStr=jug2->getNombre();
    memcpy(mensaje_a_enviar,FIRMA_DEL_PROTOCOLO,2);
    for(i=2,l=0,j=miNombreStr.size();l<j;i++){
        mensaje_a_enviar[i]=miNombreStr[l++];
    }
    int tamanioNombre;
    int bytes;
    try{
        cout<<"Conectando..."<<endl;
        socketCliente=new MySocket(PUERTO,host);
        try{
            bytes=socketCliente->read(message,BUFFER);
            socketCliente->write(mensaje_a_enviar,i);
            memcpy(firmaDelProtocolo,message,TAMANIO_FIRMA_DEL_PROTOCOLO);
            firmaDelProtocolo[TAMANIO_FIRMA_DEL_PROTOCOLO]='\0';
            bandera=message[2];
            //bandera=ntohs(bandera);
            if(bandera!=INICIO_PARTIDA || strcmp(firmaDelProtocolo,FIRMA_DEL_PROTOCOLO)!=0){
                cout<<"No se recibio la bandera adecuada"<<endl;
                socketCliente->shutdown();
                socketCliente->close();
                socketServidor->close();
                return;
            }
            versionDelProtocolo=message[3];
            memcpy(nombreDelJugador,&message[4],tamanioNombre=bytes-3);
            nombreDelJugador[tamanioNombre]='\0';
            cout<<"Firma del protocolo:"<<firmaDelProtocolo<<endl;
            cout<<"Bandera:"<<bandera<<endl;
            cout<<"Version del protocolo:"<<(unsigned short)versionDelProtocolo<<endl;
            cout<<"Nombre del jugador al que se ha conectado:"<<nombreDelJugador<<endl;
            jug1->setHost(socketCliente->getIp());
            cout<<"IP:"<< jug1->getHost()<<endl;
            jug1->setNombre(string(nombreDelJugador));

        }catch(SocketReadException& exRead){
            cout<<exRead.getMessage();
        }
    }catch(SocketException& e){
        cout<<e.getMessage()<<endl;
    }
}


void MessagesSender::iniciarPartida(){
    MySocket* socketServidor;
    MySocket* socketCliente;
    Jugador *jug1,*jug2;
    QMutex mutex;
    jug1=new Jugador();
    jug2=new Jugador();
    jug1->setNombre("Jose Vidal");
    socketServidor=new MySocket(PUERTO);
    socketServidor->setFlags(O_NONBLOCK);
    socketServidor->listen(1);
    int i,j,indiceMensaje,bytes;
    string nombreJugador1;
    char firmaDelProtocolo[TAMANIO_FIRMA_DEL_PROTOCOLO+1];
    char nombreDelJugador[TAMANIO_NOMBRE_JUGADOR+1];
    char mensaje[BUFFER+1];
    char mensajeRecibido[BUFFER+1];
    int tamanioNombreJugador;
    cout<<"Esperando jugador..."<<endl;
    mutex.lock();
    esperarConecciones=true;
    mutex.unlock();
    while( (socketCliente=socketServidor->accept())==nullptr )
    {
        if(!esperarConecciones){
            cout<<"Canceled"<<endl;
            delete socketServidor;
            return;
        }
    }
    mensaje[0]=FIRMA_DEL_PROTOCOLO[0];
    mensaje[1]=FIRMA_DEL_PROTOCOLO[1];
    mensaje[2]=INICIO_PARTIDA;
    mensaje[3]=VERSION_DEL_PROTOCOLO;
    nombreJugador1=jug1->getNombre();
    indiceMensaje=4;
    for(i=0,j=nombreJugador1.size();i<j;i++){
        mensaje[indiceMensaje++]=nombreJugador1.at(i);
    }
    try{
        socketCliente->write(mensaje,indiceMensaje);
        bytes=socketCliente->read(mensajeRecibido,BUFFER);
        memcpy(firmaDelProtocolo,mensajeRecibido,2);
        firmaDelProtocolo[TAMANIO_FIRMA_DEL_PROTOCOLO]='\0';
        memcpy(nombreDelJugador,&mensajeRecibido[2],tamanioNombreJugador=bytes-2);
        nombreDelJugador[tamanioNombreJugador]='\0';
        cout<<"Firma del protocolo:"<<firmaDelProtocolo<<endl;
        cout<<"Nombre del jugador 2:"<<nombreDelJugador<<endl;
        if(strcmp(firmaDelProtocolo,FIRMA_DEL_PROTOCOLO)!=0){
            cout<<"No se recibio la bandera adecuada"<<endl;
            socketCliente->shutdown();
            socketCliente->close();
            return;
        }
        jug2->setHost(socketCliente->getIp());
        cout<<"IP:"<< jug2->getHost()<<endl;
        cout<<"Puerto:"<<socketCliente->getPort()<<endl;
        jug2->setNombre(string(nombreDelJugador));
        mutex.lock();
        MessagesSender::socketCliente=socketCliente;
        MessagesSender::socketServidor=socketServidor;
        mutex.unlock();
    }catch(SocketWriteException& ex){
        cout<<ex.getMessage()<<endl;
    }
}
