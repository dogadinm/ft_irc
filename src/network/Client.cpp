#include "../../include/network/Client.hpp"
#include <stdexcept>
// #include <mutex>  // for thread safety if needed

/* Constructor and Destructor */

Client::Client(int fd, const std::string &port, const std::string &hostname)  // port as std::string
    : _fd(fd), _port(port), _hostname(hostname), _state(UNAUTHENTICATED) //, _channel(NULL)
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

std::string     Client::get_prefix() const
{
    std::string username = _username.empty() ? "" : "!" + _username;
    std::string hostname = _hostname.empty() ? "" : "@" + _hostname;

    return _nickname + username + hostname;
}

// Channel*        Client::get_channel() const { return _channel; }


/* Setters */

void            Client::set_nickname(const std::string &nickname) { _nickname = nickname; }
void            Client::set_username(const std::string &username) { _username = username; }
void            Client::set_realname(const std::string &realname) { _realname = realname; }
void            Client::set_state(ClientState state) { _state = state; }
// void            Client::set_channel(Channel *channel) { _channel = channel; }


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
