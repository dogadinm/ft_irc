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


    std::string name = args[0];
    std::string pass = args.size() > 1 ? args[1] : "";

    Channel* current_channel = client->get_channel();
    if (current_channel)
    {
        client->reply(ERR_TOOMANYCHANNELS(client->get_nickname(), name));
        return;
    }

    Channel* channel = _server->get_channel(name);
    if (!channel)
    {
        channel = _server->create_channel(name, pass, client);
        // if (!channel)  // Check if channel creation failed
        // {
        //     client->reply(ERR_CANNOTCREATECHANNEL(client->get_nickname(), name));
        //     return;
        // }
    }

    // Check if the channel is full
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

    // Add the client to the channel
    // if (!client->join(channel))
    // {
    //     client->reply(ERR_CANNOTJOINCHANNEL(client->get_nickname(), name));  // Handle failure to join the channel
    //     return;
    // }


    // Testing oper command 
    // if(client->get_admin_access() == true)
    // {
    //    client->join(channel);
    //    channel->broadcast(client->get_nickname() + " has joined the channel");
    // }
    // else{
    //     client->reply("No");
    // }


    client->join(channel);   
    // Optionally, notify others that the client has joined the channel
    // channel->broadcast(client->get_nickname() + " has joined the channel");
}
