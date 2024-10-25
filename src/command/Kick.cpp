#include "../../include/command/Command.hpp"

Kick::Kick(Server* server) : Command(server) {}

Kick::~Kick() {}

// syntax: KICK <channel> <client> :[<message>]

void Kick::execute(Client* client, std::vector<std::string> args)
{
    if (args.size() < 2)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "KICK"));
        return;
    }

    // Extract channel name and target client
    std::string channel_name = args[0];
    std::string target = args[1];
    std::string reason = "No reason specified!";

    // If there is a reason provided (starts with ':')
    if (args.size() >= 3 && args[2][0] == ':')
    {
        reason = args[2].substr(1); // Remove leading ':'
        for (size_t i = 3; i < args.size(); i++)  // Append remaining words
        {
            reason.append(" " + args[i]);
        }
    }

    // Retrieve the channel from the server
    Channel* channel = _server->get_channel(channel_name);
    if (!channel)
    {
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), channel_name));
        return;
    }

    // Validate that the client is in the channel
    if (client->get_channel(channel_name) != channel) //!client->get_channel(channel_name) || 
    {
        client->reply(ERR_NOTONCHANNEL(client->get_nickname(), channel_name));
        return;
    }

    // Check if the client has administrative privileges in the channel
    if (channel->get_admin(client->get_nickname()) != client)
    {
        client->reply(ERR_CHANOPRIVSNEEDED(client->get_nickname(), channel_name));
        return;
    }

    // Check if the target client exists
    Client* dest = _server->get_client(target);
    if (!dest)
    {
        client->reply(ERR_NOSUCHNICK(client->get_nickname(), target));
        return;
    }

    // Check if the target client is in the channel
    if (!dest->get_channel(channel_name) || dest->get_channel(channel_name) != channel)
    {
        client->reply(ERR_USERNOTINCHANNEL(client->get_nickname(), dest->get_nickname(), channel_name));
        return;
    }
    // channel->broadcast(dest->get_nickname() + " has been kicked from the channel by " + client->get_nickname() + ". Reason: " + reason);
    // If everything is valid, kick the target client
    channel->kick(client, dest, reason);
    dest->remove_channel(channel);

    // Notify all clients in the channel about the kick
    
    if (channel->get_size() == 0)
    {
        _server->remove_channel(channel);
        log(channel->get_name() + " closed");
    }
}
