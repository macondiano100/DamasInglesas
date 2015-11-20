#include "connectionmanager.h"

bool MessagesSender::esperarConecciones;
bool MessagesSender::responderKeepAlive;
bool MessagesSender::estaConectado;
MySocket* MessagesSender::socketServidor=nullptr;
MySocket* MessagesSender::socketCliente=nullptr;
#include <QMutex>
#include <chrono>
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
    tablero=move(a);
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
    vector<char> message=creaMensajeDeTurno(nTurno,movements,partidaGanada);
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
    using Timer=chrono::high_resolution_clock;
    using TimePoint=chrono::high_resolution_clock::time_point;
    char buffer[MAX_DATAGRAM_SIZE]={};
    bool succesReading;
    static constexpr int MAX_KEEP_ALIVE=3;
    int keepAliveSent=0;
    vector<char> mensajeDeSigueVivo=creaMensajeDeSigueVivo();
    TimePoint start,current;
    start=Timer::now();
    if(socketCliente!=nullptr)
    {
        while(true)
        {
            current=Timer::now();
            try{succesReading=socketCliente->read(buffer,MAX_DATAGRAM_SIZE)>=3&&
                        strncmp(buffer,FIRMA_DEL_PROTOCOLO,2)==0;}
            catch(Exception e){succesReading=false;}
            if(succesReading)
            {
                if(buffer[2]&TURNO)
                {
                    break;
                }
                else if(buffer[2]&SI_SIGO_VIVO)
                {
                    cout<<"----Recibida respuesta de keep alive"<<endl;
                    keepAliveSent=0;
                }
                else
                {
                    cout<<"Que carajo recibi"<<endl;
                }
            }
            else
            {
                if(chrono::duration_cast<chrono::milliseconds>(current-start).count()>=10000)
                {
                    cout<<"Sending KeepAlive"<<endl;
                    start=Timer::now();
                    keepAliveSent++;
                    socketCliente->write(mensajeDeSigueVivo.data(),mensajeDeSigueVivo.size());
                    if(keepAliveSent>MAX_KEEP_ALIVE)
                    {
                        banderas=CIERRE_FORZOSO;
                        return;
                    }
                }
            }
        }
        memcpy(&banderas,&buffer[2],1);
        memcpy(&nTurno,&buffer[3],4);
        nTurno=ntohl(nTurno);
        movements=getMovementsFromMessage(buffer);
    }
    else
    {
        sleep(1);
    }
}

void MessagesSender::enviarRespuestaDeTurno(u_int8_t banderas, int numeroTurno)
{
    vector<char> message=creaMensajeDeRespuestaDeTurno(banderas,numeroTurno);
    if(socketCliente!=nullptr)
    {
        socketCliente->write(message.data(),message.size());
    }
    else
    {
        for(auto c:message) cout<<(int)c<<endl;
    }
}

void MessagesSender::esperaRespuestaDeTurno(u_int8_t &banderas, u_int32_t &numeroTurno)
{
    char buffer[MAX_DATAGRAM_SIZE]={};
    bool succesReading;
    if(socketCliente!=nullptr)
    {
        while(true)
        {
            try{
                succesReading=socketCliente->read(buffer,MAX_DATAGRAM_SIZE)
                              >=3&&strncmp(buffer,FIRMA_DEL_PROTOCOLO,2)==0;
            }
            catch(Exception e)
            {
                succesReading=false;
            }
            if(succesReading)
            {
                if(buffer[2]&RESPUESTA_DE_TURNO)break;
            }
        }
        memcpy(&banderas,&buffer[2],1);
        memcpy(&numeroTurno,&buffer[3],4);
        numeroTurno=ntohl(numeroTurno);
    }
    else
    {
        sleep(1);
    }
}

void MessagesSender::responderKeepAlives()
{
    vector<char> mensajeDeSigoVivo=creaMensajeDeSiSigoVivo();
    char buffer[MAX_DATAGRAM_SIZE+1];
    bool succesReading;
    responderKeepAlive=true;
    if(socketCliente!=nullptr)
    {
        while(responderKeepAlive)
        {
            try{succesReading=socketCliente->read(buffer,MAX_DATAGRAM_SIZE)>=3&&
                        strncmp(buffer,FIRMA_DEL_PROTOCOLO,2)==0;}
            catch(Exception e){succesReading=false;}
            if(succesReading&&buffer[2]&SIGUE_VIVO)
                {
                    cout<<"----Recibido un keep alive"<<endl;
                    socketCliente->write(mensajeDeSigoVivo.data(),mensajeDeSigoVivo.size());
                }
        }
    }
}
void MessagesSender::pararRespuestasKeepAlive()
{
    responderKeepAlive=false;
}

bool MessagesSender::hayConeccion()
{
    return estaConectado;
}

void MessagesSender::unirse_a_partida(string host){
    Jugador *jug1,*jug2;
    jug1=new Jugador(COLOR_ROJO);
    jug2=new Jugador(COLOR_BLANCO);
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
            socketCliente->setFlags(O_NONBLOCK);
            memcpy(firmaDelProtocolo,message,TAMANIO_FIRMA_DEL_PROTOCOLO);
            firmaDelProtocolo[TAMANIO_FIRMA_DEL_PROTOCOLO]='\0';
            bandera=message[2];
            //bandera=ntohs(bandera);
            if(bandera!=INICIO_PARTIDA || strncmp(firmaDelProtocolo,FIRMA_DEL_PROTOCOLO,2)!=0){
                estaConectado=false;
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
            Tablero tablero(jug1,jug2);
            ::tablero=move(tablero);
              estaConectado=true;
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
    jug1=new Jugador(COLOR_ROJO);
    jug2=new Jugador(COLOR_BLANCO);
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
            estaConectado=false;
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
        if(strncmp(firmaDelProtocolo,FIRMA_DEL_PROTOCOLO,2)!=0){
            estaConectado=false;
            cout<<"No se recibio la bandera adecuada"<<endl;
            socketCliente->close();
            return;
        }
        jug2->setHost(socketCliente->getIp());
        cout<<"IP:"<< jug2->getHost()<<endl;
        cout<<"Puerto:"<<socketCliente->getPort()<<endl;
        jug2->setNombre(string(nombreDelJugador));
        mutex.lock();
        socketCliente->setFlags(O_NONBLOCK);
        MessagesSender::socketCliente=socketCliente;
        MessagesSender::socketServidor=socketServidor;
        Tablero tablero(jug1,jug2);
        ::tablero=move(tablero);
        mutex.unlock();
        estaConectado=true;
    }catch(SocketWriteException& ex){
        cout<<ex.getMessage()<<endl;
    }
}

void MessagesSender::closeConnection()
{
    pararRespuestasKeepAlive();
    estaConectado=false;
    if(socketServidor) delete socketServidor;
    if(socketCliente)  delete socketCliente;
    socketServidor=nullptr;
    socketCliente=nullptr;
}
