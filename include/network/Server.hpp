#pragma once


#define NI_MAXSERV    32
#define NI_MAXHOST  1025

#include <sys/socket.h>
#include <netdb.h>
#include <sys/poll.h>
#include <poll.h>
#include <iostream>
#include <stdexcept>
#include <fcntl.h>
#include <string>


#include <algorithm>
#include <vector>
#include <map>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>

class Server;

#include "../log.hpp"
#include "Client.hpp"
#include "../command/Parser.hpp"

// #include "Channel.hpp"
#define MAX_CONNECTIONS 999

class Server
{
        typedef std::vector<pollfd>::iterator       plfds_iterator;
        typedef std::map<int, Client *>::iterator   client_iterator;
    private:
        int _working;
        int _socket;

        const std::string _port;
        const std::string _pass;
        const std::string _host;

        std::vector<pollfd> _plfds;
        // std::vector<Channel *>  _channels;
        std::map<int, Client *> _clients;

        Parser*                 _parser;

        Server();
        Server(Server const & copy);
        
    public:

        // Constructor and Destructor
        Server(const std::string &port, const std::string &pass);
        ~Server();

        // Start server
        void start();


        int CreateSocket();

        // Getters
        std::string get_pass() const;
        Client*         get_client(const std::string &nickname);
        
        // Handle Clients
        void            client_connect();
        void            client_disconnect(int fd);
        void            client_message(int fd);

        std::string     read_message(int fd);
};

