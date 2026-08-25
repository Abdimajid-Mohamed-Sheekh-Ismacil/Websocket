#ifndef REQUEST_H
#define REQUEST_H


#include <map>
#include <string>


class Request{
public:

    std::string method;
    std::string path;
    std::string HttpVersion;

    std::map<std::string , std::string> args;
    std::map<std::string, std::string > form;

    Request(const std::string &RowRequest,std::string &clientip);

private:
    std::string RowRequest;
    std::string clientip;
    void parse();   
    void parseArgs();
    void parseForm();    

};

#endif