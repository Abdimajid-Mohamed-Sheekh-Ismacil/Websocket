#include "Flask.h"
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <unistd.h>
#include <format>
#include <map>

std::vector<int> list;
std::map<int ,std::string> Clients; 

int handleClients(int clientfd,std::string clientip)
{
    std::string response = "";
    while(true)
    {
        char buffer[1024] = {0};
        int n = read(clientfd,buffer,sizeof(buffer));
        if(n <= 0)
        {
            close(clientfd);
            std::cout << "DISCONNECTED : " << clientfd << std::endl;
            std::erase(list,clientfd);
            Clients.erase(clientfd);
            return 1;
        }
        std::string request = buffer;
        std::cout << "Client " << clientip << " Message: " << request ;
        for(auto [fd,ip] : Clients)
        {
            std::string message = std::format("Client {} {} : {}",clientfd,ip,request);
            if(fd != clientfd)
            {
                write(fd,message.c_str(),message.size());
            }
        }
    }
    return 0;
}

void terminalinput()
{

    while(true)
    {
        std::cout << "Choose commands: \n1.List Clients\n2.Send data \n3.Close connection"<< std::endl;

        std::string input;
        std::cin >> input;
        if(input == "1")
        {
            if(list.size() == 0)
            {
                std::cout << "\nList is Empty\n" << std::endl;
                continue;
            }
            else 
            {
                std::cout << "==================All CLients======" << std::endl;
                for(int i = 0; i < list.size(); i++)
                {
                    std::cout << "Client" << i+1 << ": " << list[i] << std::endl;
                }

                std::cout << "===================================" << std::endl;
                continue;
            }

        }
        else if(input == "2")
        {
            std::string n ;
            std::cout << "1.All clients\n2.Specific clients" << std::endl;
            std::cin >> n;       
            std::string intro = "Server Message: ";

            if(n == "1")
            {
                std::string msg;
                std::cout << "Enter message: ";
                std::cin.ignore();
                std::getline(std::cin,msg);
                msg += "\n";
                intro += msg;
                
                for(auto fd : list)
                {
                    write(fd,intro.c_str(),intro.size());
                }
            }
            else if(n  == "2") {
                int clientfd;
                std::cout << "Enter clientfd: ";
                std::cin >> clientfd;

                std::string msg;
                std::cout << "Enter message: ";
                std::cin.ignore();
                std::getline(std::cin,msg);

                msg += "\n";
                intro += msg;
                write(clientfd,intro.c_str(),intro.size());
            }
        }
        else if(input == "3")
        {
            int chose;
            std::cout << "\n1.Close one connection \n2.Close all connections\n: ";
            std::cin >> chose;

            if(chose == 1)
            {
                int clientfd;
                std::cout << "Enter ClientFd: ";
                std::cin >> clientfd;
                std::cout << clientfd << std::endl;
                std::erase(list,clientfd);
                close(clientfd);
            }
            else if (chose == 2)
            {
                for(auto fd : list)
                {
                    close(fd);

                }
                list.clear();
            }
        }
    }
}


int main()
{
    Flask server("127.0.0.1",5000);
    std::thread input(terminalinput);
    input.detach();
    while(true)
    {

        int clientfd = server.Accept();
        std::string clientip = server.clientip;
        std::string response = "Hi,Welcome to the Server.\n";
        write(clientfd,response.c_str(),response.size());

 

        list.push_back(clientfd);
        Clients[clientfd] = clientip;
        //broadcast 1
        for(auto [fd,ip] : Clients) 
        {
            std::string msg = std::format("Client {} is online.\n",ip);
            if(clientfd != fd)
            {
                write(clientfd,msg.c_str(),msg.size());
            }
        }
        //broadcast 2
        std::string msg = std::format("Client {} is online.\n",clientip);
        for(auto [fd,ip] : Clients)
        {
            if(fd != clientfd)
            {
                write(fd,msg.c_str(),msg.size());
            }
        }
        
        std::thread t(handleClients,clientfd,clientip);
        t.detach();
    }
}
