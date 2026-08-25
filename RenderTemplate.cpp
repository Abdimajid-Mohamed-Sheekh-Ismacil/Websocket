#include "RenderTemplate.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>


std::string getStatusMessage(int code)
{
    std::map<int, std::string> status;
    
    status[200] = "OK";
    status[404] = "Not Found";
    status[500] = "Internal Server Error";
    status[400] = "Bad Request";
    status[403] = "Forbidden";
    status[201] = "Created";
    status[301] = "Moved Permanently";

    if(status.count(code) > 0)  
    {
        return status[code];
    }
    return "unknown";

}



std::string renderTemplate(std::string filename,int status_code)
{
    //read file

    std::string body = "";
    std::string line;

    std::ifstream file(filename);
    if(file.is_open())
    {
       while(getline(file,line))
       {
            body += line;
       }    

    }
    else {
        body = "<h1>Not Found</h1>";
        status_code = 404;
    }

    file.close();
    //build httpresponse

    std::string httpResponse;
    int length = body.size();

    std::string headers = "HTTP/1.1 " + std::to_string(status_code) + " " + getStatusMessage(status_code) + "\r\n";
    headers += "Content-Type: text/html\r\n" ;
    headers += "Content-Length: " + std::to_string(length) + "\r\n";
    headers += "\r\n\r\n";

    httpResponse += headers + body;
    //return httpresponse
    std::cout << status_code << " -" << std::endl;
    return httpResponse;
}

