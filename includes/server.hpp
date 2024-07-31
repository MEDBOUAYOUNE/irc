#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"

struct Parse
{
    int     port;
    std::string  password;
    std::string  IPaddr;
};


// class Server{
//     private:
//         int ServerPort;
//         std::string ServerPassword;
// }
class Server
{
    private :
        Parse           pars;
        int             socketfd;
        int             epollFd;
        bool            flag;
        epoll_event     events[1024];
        int             epollCounter;
        int             communicationFD;
        sockaddr_in     addr;
        epoll_event     newClient;
        socklen_t       socklen;
        std::map<int, Parse> clients;
        std::string          str;
        char            buffer[1024];
    
    public :
        Server(char **av);
        void    serverSockCreate();
        void    startCommunication();
        void    createNewConnection();
        ~Server();
};

#endif