#ifndef EXCEPTION_H_INCLUDED
#define EXCEPTION_H_INCLUDED


#include <iostream>

using namespace std;

class Exception:public exception{
    string message;
public:
    Exception(string message){
        this->message=message;
    }
    /*~Exception(){
    }*/
    string getMessage(){
        return message;
    }
    virtual const char* what() const throw(){
        return message.c_str();
    }
};
#endif // EXCEPTION_H_INCLUDED
