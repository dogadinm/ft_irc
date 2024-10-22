#include "../../include/command/Command.hpp"

Kill::Kill(Server* server) : Command(server) {}

Kill::~Kill() {}

// syntax: KILL <nickname> <message>

void Kill::execute(Client* client, std::vector<std::string> args)
{
    std::string target = args[0];
    std::string reason = "No reason specified!";

    if (args.size() < 2)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "KILL"));
        return;
    }
    // if there is a reason provided (starts with ':')
    if (args.size() >= 2)
    {
        reason = args[1];
        for (size_t i = 2; i < args.size(); i++)  // append remaining words
        {
            reason.append(" " + args[i]);
        }
    }

    Client* dest = _server->get_client(target);
    if (!dest)
    {
        client->reply(ERR_NOSUCHNICK(client->get_nickname(), target));
        return;
    }

    if(client->get_admin_access() == false)
    {
        client->reply(ERR_NOPRIVILEGES(client->get_nickname()));
        return; 
    }

    std::string killMessage = ":" + client->get_nickname() + " KILL " + dest->get_nickname() + " :" + reason + "\n";
    _server->broadcast(killMessage);   

    std::vector<Channel*> channels = dest->get_channels();
    _server->client_disconnect(dest->get_fd());
    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        Channel* channel = *it;
        if (channel->get_size() == 0)
        {
            _server->remove_channel(channel);
            log(channel->get_name() + " closed");
        }
            
    }
}