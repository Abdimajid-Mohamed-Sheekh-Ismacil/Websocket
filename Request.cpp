#include "request.h"
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>


//private methods


void Request::parse()
{
    std::stringstream ss(RowRequest);
// cin
    ss >> method;
    ss >> path; 
    ss >> HttpVersion;

    std::time_t now = time(0);
    std::tm *t = localtime(&now);

    char buffer[1024];

    std::strftime(buffer,sizeof(buffer),"[%d/%B/%Y %H:%M:%S]",t);
    


    std::cout << buffer << " "<< "\"" << method << " " << path << " " << HttpVersion << "\""  <<" ";
}

void Request::parseArgs()
{
    if(RowRequest.find("?") != std::string::npos)
    {
        int start = RowRequest.find("?");
        std::string sub = RowRequest.substr(start);
        int end = sub.find(" ");
        std::string last = sub.substr(1,end);
        std::stringstream ss(last);

        std::string token;
        std::string key;
        std::string value;

        while(getline(ss,token,'&'))
        {
            std::stringstream ss2(token);
            getline(ss2,key,'=');
            if(token.find("=") != std::string::npos)
            {
                value = token.substr(token.find("=") + 1);
            }
            args[key] = value;
        }
    }
}

void Request::parseForm()
{
    if(RowRequest.find("\r\n\r\n") != std::string::npos)
    {
        std::string body;
        int start = RowRequest.find("\r\n\r\n") + 4;
        body = RowRequest.substr(start);

        std::stringstream ss(body);

        std::string token;
        std::string key;
        std::string value;

        while(getline(ss,token,'&'))
        {
            std::stringstream ss2(token);
            getline(ss2,key,'=');
            if(token.find("=") != std::string::npos)
            {
                value = token.substr(token.find("=") + 1);
            }
            form[key] = value;
        }
    }
}


//constructor
Request::Request(const std::string &RowRequest,std::string &clientip) : RowRequest(RowRequest),clientip(clientip)
{
    std::cout << clientip <<" - - " ;
    parse();

    if (method == "GET")
    {
        parseArgs();
    }

    else if(method == "POST")
    {
        parseForm();
    }
}