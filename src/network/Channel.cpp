#include "../../include/network/Channel.hpp"


/* Constructor and Destructor */

Channel::Channel(const std::string& name, const std::string& key, Client* admin)
    : _name(name), _admin(admin), _k(key), _l(0), _n(false)
{
    _operators.push_back(_admin);
}

Channel::~Channel() {}


/* Getters */

std::string                 Channel::get_name() const { return _name; }
Client*                     Channel::get_admin(std::string name) const 
{ 
    for (std::vector<Client *>::const_iterator it = _operators.begin(); it  != _operators.end(); ++it)
    {
        Client* client = *it;
        if(client->get_nickname() == name)
            return *it;
    }
    return NULL; 
}

std::string                 Channel::get_key() const { return _k; }
size_t                      Channel::get_limit() const { return _l; }
bool                        Channel::ext_msg() const { return _n; }
bool                        Channel::is_invite_only() const { return _i; }
bool                        Channel::is_topic_restricted() const { return _t; }

size_t                      Channel::get_size()const { return _clients.size(); }
std::string                 Channel::get_topic() const { return _topic; }
std::vector<Client *>       Channel::get_clients() const {return _clients ;}

std::vector<std::string>    Channel::get_nicknames() const
{
    std::vector<std::string> nicknames;

    std::vector<Client *>::const_iterator it_b = _clients.begin();
    std::vector<Client *>::const_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        Client* client = *it_b;

        std::string nick = (client == _admin ? "@" : "") + client->get_nickname();
        nicknames.push_back(nick);

        it_b++;
    }

    return nicknames;
}

Client*   Channel::get_client(std::string name)
{
    std::vector<std::string> nicknames;

    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        Client* client = *it_b;
        if (!name.compare((*it_b)->get_nickname()))
            return (*it_b);
        it_b++;
    }
    return NULL;
}


/* Setters */

void                        Channel::set_key(std::string key) { _k = key; }
void                        Channel::set_limit(size_t limit) { _l = limit; }
void                        Channel::set_ext_msg(bool flag) { _n = flag; }
void                        Channel::set_invite_only(bool flag) { _i = flag; }
void                        Channel::set_topic_restricted(bool flag) { _t = flag; }
void                        Channel::set_topic(const std::string& newTopic) {_topic = newTopic; }

/* Channel Actions */

void                        Channel::broadcast(const std::string& message)
{
    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        (*it_b)->write(message);
        it_b++;
    }
}

void                        Channel::broadcast(const std::string& message, Client* exclude)
{
    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        if (*it_b == exclude)
        {
            it_b++;
            continue;
        }

        (*it_b)->write(message);
        it_b++;
    }
}

void                        Channel::add_client(Client* client)
{
    _clients.push_back(client);
}

void                        Channel::remove_client(Client* client)
{
    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        if (*it_b == client)
            _clients.erase(it_b);
        it_b++;
    }
    // client->remove_channel(this);

    // for (client_iterator it = _clients.begin();it != _clients.end();)
    // {
    //     if (*it == client)
    //         it = _clients.erase(it);
    // }
    
    // client->set_channel(NULL);
    log(client->get_nickname() + " left channel " + _name);   
    if (get_admin(client->get_nickname()) && !_clients.empty())
    {
        _operators.push_back(*(_clients.begin()));

        std::string message = _admin->get_nickname() + " is now the admin of the channel " + _name;
        log(message);
    }
}

void                        Channel::kick(Client* client, Client* target, const std::string& reason)
{
    this->broadcast(RPL_KICK(client->get_prefix(), _name, target->get_nickname(), reason));
    this->remove_client(target);

    std::string message = client->get_nickname() + " kicked " + target->get_nickname() + " from channel " + _name;
    log(message);
}



void Channel::add_operator(Client* client)
{
    if (std::find(_operators.begin(), _operators.end(), client) == _operators.end())
    {
        _operators.push_back(client);
        std::string message = client->get_nickname() + " is now an operator of channel " + _name;
        log(message);
    }
}

void Channel::remove_operator(Client* client)
{
    client_iterator it = std::find(_operators.begin(), _operators.end(), client);
    if (it != _operators.end())
    {
        _operators.erase(it);
        std::string message = client->get_nickname() + " is no longer an operator of channel " + _name;
        log(message);
    }
}
