#ifndef SOCKETEXCEPTIONS_H_INCLUDED
#define SOCKETEXCEPTIONS_H_INCLUDED

#include "Exception.h"

class SocketException:public Exception{
public:
    SocketException(string msg):Exception(msg){
    }
};
class ConnectSocketException:public SocketException{
public:
    ConnectSocketException(string msg):SocketException(msg){
    }
};class SocketReadException:public SocketException{
public:
    SocketReadException(string msg):SocketException(msg){
    }
};
class SocketWriteException:public SocketException{
public:
    SocketWriteException(string msg):SocketException(msg){
    }
};
class BindSocketException:public SocketException{
public:
    BindSocketException(string msg):SocketException(msg){
    }
};
class ListenSocketException:public SocketException{
public:
    ListenSocketException(string msg):SocketException(msg){
    }
};

#endif // SOCKETEXCEPTIONS_H_INCLUDED
