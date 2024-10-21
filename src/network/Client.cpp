#include "../../include/network/Client.hpp"
#include <stdexcept>
// #include <mutex>  // for thread safety if needed

/* Constructor and Destructor */

Client::Client(int fd, const std::string &port, const std::string &hostname)  // port as std::string
    : _fd(fd), _port(port), _hostname(hostname), _state(UNAUTHENTICATED), _channels(0), _admin_access(false) //, _channel(NULL)
{

}


Client::~Client()
{
    // Clean up _channel if Client is responsible for it (assumes external management here)
    // delete _channel; // Uncomment if dynamically allocated and Client manages it
}


/* Getters */

int             Client::get_fd() const { return _fd; }
std::string     Client::get_port() const { return _port; }  // Returning port as std::string

std::string     Client::get_nickname() const { return _nickname; }
std::string     Client::get_username() const { return _username; }
std::string     Client::get_realname() const { return _realname; }
std::string     Client::get_hostname() const { return _hostname; }
ClientState     Client::get_state() const { return _state; }
bool            Client::get_admin_access() const {return _admin_access; }

std::string     Client::get_prefix() const
{
    std::string username = _username.empty() ? "" : "!" + _username;
    std::string hostname = _hostname.empty() ? "" : "@" + _hostname;
    std::string nickname = _admin_access ? "@" + _nickname : _nickname;

    return nickname + username + hostname;
}

Channel*        Client::get_channel(std::string name) const 
{
    for (std::vector<Channel *>::const_iterator  it = _channels.begin(); it != _channels.end(); ++it)
    {
        if (*it == NULL) // Check NULL
            continue;
        if (!name.compare((*it)->get_name()))
            return (*it);
    }
    return NULL;

}


/* Setters */

void            Client::set_nickname(const std::string &nickname) { _nickname = nickname; }
void            Client::set_username(const std::string &username) { _username = username; }
void            Client::set_realname(const std::string &realname) { _realname = realname; }
void            Client::set_state(ClientState state) { _state = state; }
void            Client::set_channel(Channel *channel) { _channels.push_back(channel);; }
void            Client::set_admin_access(bool status) {_admin_access = status; }


/* Check State */

bool            Client::is_registered() const { return _state == REGISTERED; }


/* Send/Receive Actions */

void            Client::write(const std::string& message) const
{
    std::string buffer = message + "\r\n";
    if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
    {
        perror("send");  // Output the specific error
        throw std::runtime_error("Error while sending a message to a client!");
    }
}

void            Client::reply(const std::string& reply)
{
    this->write(":" + get_prefix() + " " + reply);
}

void            Client::welcome()
{
    if (_state != AUTHENTICATED || _username.empty() || _realname.empty() || _nickname.empty())\
       return; 
        

    _state = REGISTERED;
    this->reply(RPL_WELCOME(_nickname));

    char message[150];  // Increased size to account for string port
    sprintf(message, "%s:%s is now known as %s.", _hostname.c_str(), _port.c_str(), _username.c_str());  // Using %s for port
    log(message);
}


/* Client Actions */

// void            Client::join(Channel* channel)
// {
//     // std::lock_guard<std::mutex> lock(_mutex);  // Thread-safe access

//     // channel->add_client(this);
//     // _channel = channel;

//     // Get users on the channel

//     std::string users = "";
//     std::vector<std::string> nicknames = channel->get_nicknames();
//     std::vector<std::string>::iterator it_b = nicknames.begin();
//     std::vector<std::string>::iterator it_e = nicknames.end();
//     while (it_b != it_e)
//     {
//         users.append(*it_b + " ");
//         it_b++;
//     }

//     // Send replies

//     // reply(RPL_NAMREPLY(_nickname, channel->get_name(), users));
//     // reply(RPL_ENDOFNAMES(_nickname, channel->get_name()));
//     // channel->broadcast(RPL_JOIN(get_prefix(), channel->get_name()));

//     // log

//     std::string message = _nickname + " has joined the channel " + channel->get_name();
//     log(message);
// }

// void            Client::leave()
// {
//     std::lock_guard<std::mutex> lock(_mutex);  // Thread-safe access

//     if (!_channel)
//         return;

//     const std::string name = _channel->get_name();

//     _channel->broadcast(RPL_PART(get_prefix(), _channel->get_name()));
//     _channel->remove_client(this);

//     std::string message = _nickname + " has left the channel " + name;
//     log(message);
// }


// void            Client::join(Channel* channel)
// {
//     channel->add_client(this);
//     _channel = channel;

//     // Get users on the channel

//     std::string users = "";
//     std::vector<std::string> nicknames = channel->get_nicknames();
//     std::vector<std::string>::iterator it_b = nicknames.begin();
//     std::vector<std::string>::iterator it_e = nicknames.end();
//     while (it_b != it_e)
//     {
//         users.append(*it_b + " ");
//         it_b++;
//     }

//     // Send replies
    
//     reply(RPL_NAMREPLY(_nickname, channel->get_name(), users));
//     reply(RPL_ENDOFNAMES(_nickname, channel->get_name()));
//     channel->broadcast(RPL_JOIN(get_prefix(), channel->get_name()));

//     // log

//     std::string message = _nickname + " has joined to the channel " + channel->get_name();
//     log(message);
// }

// void            Client::leave()
// {
//     if (!_channel)
//         return;

//     const std::string name = _channel->get_name();

//     _channel->broadcast(RPL_PART(get_prefix(), _channel->get_name()));
//     _channel->remove_client(this);

//     std::string message = _nickname + " has left the channel " + name;
//     log(message);
// }


void Client::join(Channel* channel)
{
    // Check if already in the channel
    if (std::find(_channels.begin(), _channels.end(), channel) == _channels.end())
    {
        _channels.push_back(channel);  // Add to list of channels
        channel->add_client(this);     // Assuming this method adds the client to the channel

        // Send user list and broadcast the join event
        std::string users = "";
        std::vector<std::string> nicknames = channel->get_nicknames();
        for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); ++it)
        {
            users.append(*it + " ");
        }

        // Notify client of the current users in the channel
        reply(RPL_NAMREPLY(_nickname, channel->get_name(), users));
        reply(RPL_ENDOFNAMES(_nickname, channel->get_name()));

        // Broadcast join message to the channel
        channel->broadcast(RPL_JOIN(get_prefix(), channel->get_name()));
        log(_nickname + " has joined the channel " + channel->get_name());
    }
}

void Client::leave(Channel* channel)
{
    std::vector<Channel*>::iterator it = std::find(_channels.begin(), _channels.end(), channel);
    if (it != _channels.end())
    {
        _channels.erase(it);  // Remove from the list of channels
        channel->remove_client(this);  // Assuming this method removes the client from the channel

        // Notify channel of departure
        channel->broadcast(RPL_PART(get_prefix(), channel->get_name()));

        log(_nickname + " has left the channel " + channel->get_name());
    }
}
// void Client::leave_all_channels()
// {
        
//     // for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
//     // {
        
//     //     Channel* channel = *it;
//     //     std::cout << *it << std::endl;   
//     // }
    
//     for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
//     {

//         Channel* channel = *it;

//         channel->remove_client(this);

//         channel->broadcast(RPL_PART(get_prefix(), channel->get_name()));

//         log(_nickname + " has left the channel " + channel->get_name());

//         _channels.erase(it);

//     }
// }

void Client::leave_all_channels()
{
    for (size_t i = 0; i < _channels.size();)
    {
        Channel* channel = _channels[i]; 
        if (channel != NULL)
        {
            channel->remove_client(this); 
            channel->broadcast(RPL_PART(get_prefix(), channel->get_name()));
            log(_nickname + " has left the channel " + channel->get_name());
            _channels.erase(_channels.begin() + i); 
        }
        else
            ++i;
    }
}

int Client::get_channel_count() const
{
    return _channels.size();  // Return the number of channels the client is in
}
