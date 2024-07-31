#include "../includes/irc.hpp"

void    parse(char **av)
{
    int x = -1;
    while (av[2][++x])
    {
        if (!isalnum(av[2][x]))
            throw exception();
    }
    x = -1;
    while (av[1][++x])
    {
        if (!isdigit(av[1][x]))
            throw exception();
    }
    if (atoi(av[1]) < 1024 && atoi(av[1]) > 65535)
        throw exception();
}