#include "../../include/network/Server.hpp"

Server::Server(const std::string &port, const std::string &pass) : _host("127.0.0.1"), _port(port), _pass(pass), admin_name("dogadinm"), admin_pass("12345")
{
    _working = 1;
    _socket = CreateSocket();
    _parser = new Parser(this);
}

int Server::CreateSocket()
{
    int server_fd;
    struct sockaddr_in server_addr;  

    // Create the server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        throw std::runtime_error("Error, opening socket");


    // Set up the server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // Set the address family to IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all available interfaces
    server_addr.sin_port = htons(atoi(_port.c_str())); // Convert the port to network byte order

    // Allow port reuse
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
        throw std::runtime_error("Error, socket options");

    // Make socket NON-BLOCKING
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK))
        throw std::runtime_error("Error, NON-BLOCKING doenst set");
    
    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        throw std::runtime_error("Error, binding socket");

    // Listen for incoming connections
    if (listen(server_fd, MAX_CONNECTIONS) < 0)
        throw std::runtime_error("Error, listening socket");

    return server_fd; 
}

void Server::start()
{
    pollfd srv = {_socket, POLLIN, 0};
    _plfds.push_back(srv);

    log("Server listening on port " + _port);

    while (_working)
    {
        if (poll(_plfds.data(), _plfds.size(), -1) < 0)
            throw std::runtime_error("Error, polling!");
        // for (size_t i = 0; i < _plfds.size(); ++i) {
        //     std::cout << "Index: " << i << ", fd: " << _plfds[i].fd
        //                 << ", events: " << _plfds[i].events
        //                 << ", revents: " << _plfds[i].revents << std::endl;
        // }
        for (plfds_iterator it = _plfds.begin(); it != _plfds.end(); ++it)
        {
            // std::cout << it->revents << std::endl;
            if (it->revents == 0) {
                continue;
            }

            // Checking for a broken connection
            if (it->revents & POLLHUP || it->revents & POLLRDHUP)
            {
                // std::cout << "POLLRDHUP" << std::endl;
                client_disconnect(it->fd);
                break;  
            }

            // Processing incoming data
            if (it->revents & POLLIN)
            {
                // std::cout << "POLLIN" << std::endl;
                if (it->fd == _socket)
                {
                    this->client_connect();
                    break;
                }
                this->client_message(it->fd); 
            }
        }
    }
}

void Server::client_connect()
{
    int client_fd;
    sockaddr_in  client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Accept client fd
    client_fd = accept(_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0)
        throw std::runtime_error("Error, accepting a new client!");


    // Adding the client fd in the poll
    pollfd cln = {client_fd, POLLIN, 0};
    _plfds.push_back(cln);


    // Getting hostname from the client address
    char hostname[NI_MAXHOST];
    char service[NI_MAXSERV];
    int res = getnameinfo((sockaddr *) &client_addr, sizeof(client_addr), hostname, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
    if (res != 0)
        throw std::runtime_error("Error, getting a hostname!");\
   
    std::string host_str(hostname);
    std::string serv_str(service);

    // uint16_t cln_port = ntohs(client_addr.sin_port); // port 



    
    Client* client = new Client(client_fd, serv_str, host_str);
    _clients.insert(std::make_pair(client_fd, client));

    // _clients.insert(std::make_pair(client_fd, serv_str));
    log(host_str + ":" + serv_str + " connected");
}

void Server::client_disconnect(int fd)
{
    try
    {
        
        // need to make leave from the channel when user exit
        if (_clients.find(fd) == _clients.end())
            return;
        Client* client = _clients.at(fd);
        client->leave_all_channels();    
        // std::string clientData = _clients.at(fd);
        log(client->get_hostname() + " : " + client->get_port() + " disconnected");
        
        _clients.erase(fd);
        for(plfds_iterator it = _plfds.begin(); it != _plfds.end(); ++it)
        {
            if (it->fd == fd)
            {
                _plfds.erase(it);
                close(fd);
                break;
            }
        }
        delete client;
    }
    catch(const std::exception& e)
    {
        std::cout << "Error while disconnecting! " << e.what() << std::endl;
    }
    

}

void Server::client_message(int fd)
{

    try
    {
        if (_clients.find(fd) == _clients.end())
            return;
        Client*     client = _clients.at(fd);
       
        std::string message = this->read_message(fd);
        // if (message == "QUIT")    
        //     client_disconnect(fd); 
              
        // log(clientData + ": " + message);
        
        _parser->invoke(client, message);
    }
    catch (const std::exception& e) 
    {
        std::cout << "Error while handling the client message! " << e.what() << std::endl;
    }
}

std::string Server::read_message(int fd)
{
    std::string message;
    
    char buffer[1024];
    bzero(buffer, 1024);

    while (!strstr(buffer, "\n"))
    {
        bzero(buffer, 100);
        ssize_t bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);

        if ((bytes_read < 0))
            throw std::runtime_error("Error while reading buffer from a client!");
        if (bytes_read == 0) {
            return "QUIT";
        }
        buffer[bytes_read] = '\0';
        message.append(buffer);
        
    }
    return message;
}



Server::~Server()
{
    if (_socket >= 0) 
        close(_socket);

    delete _parser;
}


std::string Server::get_pass() const    { return _pass; }


Client*         Server::get_client(const std::string& nickname)
{
    
    for (client_iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (!nickname.compare(it->second->get_nickname()))
            return it->second; 
    }
    return NULL;
}


Channel*        Server::get_channel(const std::string& name)
{
    for (channel_iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (*it == NULL) // Check NULL
            continue;
        if (!name.compare((*it)->get_name()))
            return (*it);
    }
    return NULL;
}

Channel*        Server::create_channel(const std::string& name, const std::string& key, Client* client)
{
    Channel *channel = new Channel(name, key, client);
    _channels.push_back(channel);

    return channel;
}

void Server::broadcast(const std::string& message)
{
    int fd_cln;
    for (client_iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        fd_cln = it->second->get_fd();
        if (send(fd_cln, message.c_str(), message.length(), 0) < 0)
            throw std::runtime_error("Error send a message from server!");
    }
    
}


void Server::remove_channel(Channel* channel)
{
    for(channel_iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (*it == channel)
        {
            _channels.erase(it);
            break;
        }
    }

}



std::string     Server::get_admin_name() { return admin_name; }
std::string     Server::get_admin_pass() { return admin_pass; }