#include "../../include/command/Command.hpp"

Part::Part(Server* server) : Command(server) {}

Part::~Part() {}

// syntax: PART <channels> [<message>]

void Part::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "PART"));
        return;
    }

    std::string name = args[0];
    std::string partMessage = (args.size() > 1) ? args[1] : client->get_nickname() + " has left the channel";  // Default message if not provided

    // Find the channel by name
    Channel* channel = _server->get_channel(name);

    // Check if the client is actually in the target channel
    if (!channel || !channel->get_client(client->get_nickname()))
    {
        client->reply(ERR_NOTONCHANNEL(client->get_nickname(), name));
        return;
    }

    // Broadcast the part message to the channel
    channel->broadcast(RPL_PART(client->get_nickname(), name));

    // Remove the client from the channel
    channel->remove_client(client);
    client->remove_channel(channel);

    // If the channel is empty after the client leaves, remove the channel (optional)
    if (channel->get_size() == 0)
    {
        _server->remove_channel(channel);
        log(channel->get_name() + " closed");
    }
}
