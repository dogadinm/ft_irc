#include "../../include/command/Command.hpp"

Notice::Notice(Server* server) : Command(server) {}

Notice::~Notice() {}

// syntax: NOTICE <msgtarget> :<message>

void Notice::execute(Client* client, std::vector<std::string> args)
{
    if (args.size() < 2 || args[0].empty() || args[1].empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "NOTICE"));
        return;
    }

    // Extract target and the message
    std::string target = args.at(0);
    std::string message = args[1].at(0) == ':' ? args[1].substr(1) : args[1];

    // Concatenate remaining arguments into the notice message
    for (size_t i = 2; i < args.size(); ++i)
    {
        message.append(" " + args[i]);
    }

    // If the notice is intended for a channel
    if (target.at(0) == '#')
    {
        Channel* channel = _server->get_channel(target);

        // Channel not found
        if (!channel)
        {
            client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), target));
            return;
        }

        // Check if the channel accepts external messages or if the client is in the channel
        if (!channel->ext_msg() && !channel->get_client(client->get_nickname()))
        {
            client->reply(ERR_CANNOTSENDTOCHAN(client->get_nickname(), target));
            return;
        }

        // Broadcast message to the channel, excluding the sender
        channel->broadcast(RPL_NOTICE(client->get_prefix(), target, message), client);
        return;
    }

    // If the message is for a specific client
    Client* dest = _server->get_client(target);
    if (!dest)
    {
        client->reply(ERR_NOSUCHNICK(client->get_nickname(), target));
        return;
    }

    // Send the private message to the target client
    dest->write(RPL_NOTICE(client->get_prefix(), target, message));
}
