#include "../includes/irc.hpp"

Server::Server(char **av)
{
    this->pars.port = atoi(av[1]);
    this->pars.password = av[2];
    this->pars.IPaddr = "127.0.0.1";
    if ((this->epollFd = epoll_create(1)) < 0)
        throw exception();
    this->flag = false;
    this->serverSockCreate();
    this->startCommunication();
}

Server::~Server(){}

void    Server::serverSockCreate()
{
    if ((this->socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        perror("socket() error"), throw exception();
    int optval = 1;
    if (setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        perror("setsockopt() error"), throw exception();
    memset(&this->addr, 0, sizeof(addr));
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(this->pars.port);
    if (inet_pton(AF_INET, this->pars.IPaddr.c_str(), &this->addr.sin_addr.s_addr) < 0)
        perror("inet_pton() error"), throw exception();
        
    if (bind(this->socketfd, &(sockaddr &)addr, sizeof(addr)) < 0)
        perror("bind() error"), throw exception();
    if (listen(this->socketfd, 10) < 0)
        perror("listen() error"), throw exception();
    this->newClient.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;
    this->newClient.data.fd = this->socketfd;
    if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, this->socketfd, &newClient) < 0)
        perror("epoll_ctl() error"), throw exception();
}

void    Server::createNewConnection()
{
    memset(&addr, 0, sizeof(addr));
    this->socklen = sizeof(addr);
    if ((this->communicationFD = accept(this->socketfd, &(sockaddr &)this->addr, &this->socklen)) < 0)
        perror("accept() error"), throw exception();
    memset(&this->newClient, 0, sizeof(this->newClient));
    this->newClient.data.fd = this->communicationFD;
    this->newClient.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP | EPOLLRDHUP;
    if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, this->communicationFD, &this->newClient) < 0)
        perror("epoll_ctl() error"), throw exception();
    clients[communicationFD] = this->pars;
}

void    Server::startCommunication()
{
    int x = 0;
    while (1)
    {
        this->epollCounter = epoll_wait(this->epollFd, this->events, 102, -1);
        if (this->epollCounter < 0)
        {
            cout << "epollfd : " << this->epollCounter << endl;
            perror("epoll_wait() error"), throw exception();
        }
        x = -1;
        while (++x < this->epollCounter)
        {
            this->flag = false;
            if (this->events[x].data.fd == this->socketfd)
                this->createNewConnection();
            if (this->events[x].events & EPOLLERR || this->events[x].events & EPOLLHUP || this->events[x].events & EPOLLRDHUP)
            {
                if (epoll_ctl(this->epollFd, EPOLL_CTL_DEL, events[x].data.fd, events) > 0)
                    perror("epoll_ctl() error"), throw exception();
                close(events[x].data.fd);
                this->clients[events[x].data.fd];
            }
            if (events[x].events & EPOLLIN && events[x].data.fd != socketfd)
            {
                memset(buffer, 0, 1024);
                recv(events[x].data.fd, buffer, 1024, 0);
                cout << "in fd: "<< events[x].data.fd << " " << buffer;
            }
            // if (events[x].events & EPOLLOUT && events[x].data.fd != socketfd && this->flag == true)
            // {
            //     cin >> str;
            //     send(events[x].data.fd, str.c_str(), str.size(), 0);
            //     this->flag = false;
                // if (epoll_ctl(this->epollFd, EPOLL_CTL_DEL, events[x].data.fd, events) > 0)
                //     perror("epoll_ctl() error"), throw exception();
                // close(events[x].data.fd);
                // this->clients.erase(events[x].data.fd);
            //}
        }
    }
}