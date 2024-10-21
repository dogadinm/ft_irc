#include "../../include/command/Command.hpp"

Join::Join(Server* srv) : Command(srv) {}

Join::~Join() {}

// syntax: JOIN <channels> [<keys>]

void Join::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "JOIN"));
        return;
    }
    if (args[0][0] != '#')
    {
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), args[0]));
        return;
    }



    std::string name = args[0];
    std::string pass = args.size() > 1 ? args[1] : "";

    if (client->get_channel_count() >= 10)
    {
        client->reply(ERR_TOOMANYCHANNELS(client->get_nickname(), name));
        return;
    }

    Channel* channel = _server->get_channel(name);
    if (!channel)
        channel = _server->create_channel(name, pass, client);


    Channel* cln_channel = client->get_channel(name);
    if (cln_channel)
        return;

    if (channel->get_limit() > 0 && channel->get_size() >= channel->get_limit())
    {
        client->reply(ERR_CHANNELISFULL(client->get_nickname(), name));
        return;
    }

    // Check if the channel has a password and if it's incorrect
    if (!channel->get_key().empty() && channel->get_key() != pass)
    {
        client->reply(ERR_BADCHANNELKEY(client->get_nickname(), name));
        return;
    }

    client->join(channel);
  
    // Optionally, notify others that the client has joined the channel
    // channel->broadcast(client->get_nickname() + " has joined the channel");
}
