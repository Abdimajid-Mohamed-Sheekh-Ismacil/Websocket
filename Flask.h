#ifndef FLASK_H
#define FLASK_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>


class Flask{

public:
    Flask();
    Flask(std::string ip,unsigned int port);
    int Accept();
    bool isRunning = false;
    std::string clientip = "";
    ~Flask();


private:

    bool CreateSocket();
    void CreateSereverStruct();
    bool bindSocket();
    void Startlistening();
    bool start();


    struct sockaddr_in server;
    struct sockaddr_in client;

    unsigned int port = 5000;
    std::string ip = "127.0.0.1";
    
    int serverfd;
    int clientfd;
    
    
};

#endif