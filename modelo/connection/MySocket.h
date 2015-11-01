#ifndef MYSOCKET_H_INCLUDED
#define MYSOCKET_H_INCLUDED

#include "utilidadesVariadas.h"
#include "socketExceptions.h"

#ifdef __WIN32__
# include <winsock2.h>
#include <windows.h>

#else
#include <sys/types.h>
# include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

socklen_t TAMANIO_SOCKET_4=sizeof(sockaddr_in);
socklen_t TAMANIO_SOCKET_6=sizeof(sockaddr_in6);
#define BUF_SIZE 500
#define TAMANIO_PUERTO 64

class MySocket{
private:
    int desc;
    sockaddr_in* sock4;
    sockaddr_in6* sock6;
    addrinfo *res,*resSave;
    bool is_ipv6;
    MySocket(int desc,sockaddr_in* sock){
        setDesc(desc);
        setSock(sock);
    }
    MySocket(int desc,sockaddr_in6* sock){
        setDesc(desc);
        setSock(sock);
    }
    void setDesc(int d){
        desc=d;
    }
    void setSock(sockaddr_in* s){
        sock4=s;
    }
    void setSock(sockaddr_in6* s){
        sock6=s;
    }

public:
    MySocket(string port):MySocket(toInt(port)){
    }
    MySocket(int port)throw (SocketException){
        res=NULL;
        sock6=nullptr;
        sock4=nullptr;
        struct addrinfo hints;
       struct addrinfo *result, *rp;
       int s;
       memset(&hints, 0, sizeof(struct addrinfo));
       hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
       hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
       hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
       hints.ai_protocol = 0;          /* Any protocol */
       hints.ai_canonname = NULL;
       hints.ai_addr = NULL;
       hints.ai_next = NULL;
       s = getaddrinfo(NULL, toStr(port).c_str(), &hints, &result);
       if (s != 0) {
            throw SocketException(gai_strerror(s));
       }
       for ( rp = result; rp != NULL; rp = rp->ai_next) {
           desc = socket(rp->ai_family, rp->ai_socktype,
                   rp->ai_protocol);
           if (desc == -1)
               continue;
           if (::bind(desc, rp->ai_addr, rp->ai_addrlen) == 0)
               break;                  /* Success */
           ::close(desc);
       }
       if (rp == NULL) {               /* No address succeeded */
           throw SocketException("No se pudo enlazar el socket");
       }
       freeaddrinfo(result);           /* No longer needed */
    }
    MySocket(string port,string host)throw (SocketException){
        addrinfo hints;
        int n;
        bool seEncontroHost=false;
        memset(&hints,0,sizeof(hints));
        hints.ai_flags=AI_PASSIVE;
        hints.ai_family=AF_UNSPEC;
        hints.ai_socktype=SOCK_STREAM;
        n=getaddrinfo(host.c_str(),port.c_str(),&hints,&res);
        if(n<0){
            throw SocketException("Host no valido");
        }
        resSave=res;
        while(res!=NULL && !seEncontroHost){
            desc=socket(res->ai_family, res->ai_socktype,
                                res->ai_protocol);
            if( (is_ipv6=res->ai_family==AF_INET6)){//Conexión por IPv6
                //cout<<"Conexion por IPv6"<<endl;
            }
            else if(res->ai_family==AF_INET){//Conexión por IPv4
                //cout<<"Conexion por IPv4"<<endl;
            }
            seEncontroHost=
                    ::connect(desc, res->ai_addr, res->ai_addrlen)>=0;
            if(seEncontroHost){
                break;
            }
            else{
                res=res->ai_next;
            }

        }
        if(!seEncontroHost){
            throw ConnectSocketException("No se pudo conectar al host");
        }
    }
    MySocket(int port,string host) throw (SocketException):MySocket(toStr(port),host){

    }
    ~MySocket(){
        if(resSave!=nullptr){
            freeaddrinfo(resSave);
        }
        else if(sock6!=nullptr){
            delete sock6;

        }
        else if(sock4!=nullptr){
            delete sock4;
        }
    }
    void close()throw (SocketException){
        int x=::close(desc);
        if(x<0){
            throw SocketException("No se pudo cerrar");
        }
    }
    void shutdown()throw (SocketException){
        int x=::shutdown(desc,SHUT_RDWR);
        if(x<0){
            throw SocketException("No se pudo cerrar");
        }
    }
    void setFlags(int flags){
        fcntl(getDesc(),F_SETFL,flags);
    }
    int getFlags(){
        return fcntl(getDesc(),F_GETFL);
    }
    void addFlags(int flag){
        setFlags(getFlags()|flag);
    }
    string getIp(){
        char ip[INET6_ADDRSTRLEN];
        char puerto[TAMANIO_PUERTO+1];
        if(res!=NULL){
            getnameinfo(res->ai_addr,res->ai_addrlen,ip,INET6_ADDRSTRLEN,puerto,
                TAMANIO_PUERTO,NI_NUMERICHOST);
        }
        else if(sock4!=nullptr){
            inet_ntop(AF_INET  ,&(sock4->sin_addr),ip,sizeof ip);
        }
        else{
            inet_ntop(AF_INET6  ,&(sock6->sin6_addr),ip,sizeof ip);
        }

        return string(ip);
    }
    unsigned int getPort(){
        //cout<<"Before sock4 port"<<endl;
        if(sock4!=nullptr){
            //cout<<"sock4 is not null"<<endl;
        }
        else{
            //cout<<"sock4 is null"<<endl;
        }
        return htons(sock4==NULL?sock6->sin6_port:sock4->sin_port);
    }
    MySocket* accept(){
        sockaddr_in *sock4=new sockaddr_in;
        sockaddr_in6 *sock6=new sockaddr_in6;
        //cout<<"Antes de ipv6"<<endl;
        int desc=is_ipv6?
            ::accept (getDesc(),(sockaddr *) sock6, (socklen_t *) &TAMANIO_SOCKET_6):-1;
        if(desc<0){
            delete sock6;
            //cout<<"Buscando en ipv4"<<endl;
            desc=::accept (getDesc(),(sockaddr *) sock4, (socklen_t *) &TAMANIO_SOCKET_4);
            //cout<<"Antes del return"<<endl;
            //cout<<"desc:" << desc<<endl;
            if(desc>0){
                return new MySocket(desc,sock4);
            }
            else{
                delete sock4;
                return NULL;
            }
        }
        delete sock4;
        return desc>0?new MySocket(desc,sock6):NULL;

    }
    int write(char* message,int length)throw (SocketWriteException){
        int x=::write(desc,message,length);
        if(x<0){
            throw SocketWriteException("Ocurrio un error al escribir");
        }
        return x;
    }
    int read(char* message,int length)throw(SocketReadException){
        int x=::read(desc,message,length);
        if(x<0){
            throw SocketReadException("Ocurrio un error al leer");
        }
        return x;
    }
    int getDesc(){
        return desc;
    }
    /*sockaddr_in* getSocket(){
        return &sock;
    }*/
    void listen(int n)throw (ListenSocketException){
        int x=::listen (desc,n);
        if(x<0){
            throw ListenSocketException("Error al escuchar sockets");
        }
    }
};

#endif

#endif // MYSOCKET_H_INCLUDED
