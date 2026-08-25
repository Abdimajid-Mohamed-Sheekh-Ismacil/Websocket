#include "Flask.h"
#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>




//private methods

bool Flask::CreateSocket()
{
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0)
    {
        return false;
    }
    serverfd = fd;
    return true;
}

void Flask::CreateSereverStruct()
{
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());
}

bool Flask::bindSocket()
{
    //
    if(bind(serverfd,(struct sockaddr *)&server,sizeof(server)) < 0)
    {
        return false;
    }
    return true;
}

void Flask::Startlistening()
{
    //
    listen(serverfd,5);
}

//consturctors

Flask::Flask()
{
    //
    start();
}

Flask::Flask(std::string ip,unsigned int port) : ip{ip} ,port{port}
{
    start();
}


//public methods

bool Flask::start()
{   
    if(!CreateSocket())
    {
        return false;
        //
    }
    int value = 1; 
    setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value));
    CreateSereverStruct();
    if(!bindSocket())
    {
        return false;
    }
    Startlistening();

    isRunning = true;
    std::cout << "SERVER STARTED.....\n";
    return true;
}

int Flask::Accept()
{
    //
    socklen_t size = sizeof(client);

    clientfd = accept(serverfd,(struct sockaddr *)&client,&size);
    clientip = inet_ntoa(client.sin_addr );

    return clientfd;
}


// destructor
Flask::~Flask()
{
    close(clientfd);
    close(serverfd);
    std::cout << "SERVER CLOSED" << std::endl;
    isRunning = false;
}

