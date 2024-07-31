#include "includes/irc.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "Usage : ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    else
    {
        try
        {
            parse(av);
            Server Server(av);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}