#include "../../include/command/Command.hpp"

Invite::Invite(Server* server) : Command(server) {}

Invite::~Invite() {}

// INVITE <nick> [<channel>]

void Invite::execute(Client* client, std::vector<std::string> args) 
{
    // Check if there are enough arguments for the INVITE command
    if (args.size() < 2) {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "INVITE"));
        return;
    }

    std::string targetNickname = args[0];
    std::string channelName = args[1];

    Channel* channel = _server->get_channel(channelName);
    Client* targetClient = _server->get_client(targetNickname);

    // Check if the specified channel exists
    if (!channel) 
    {
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), channelName));
        return;
    }
    // Check if the target client exists
    if (!targetClient) 
    {
        client->reply(ERR_NOSUCHNICK(client->get_nickname(), targetNickname));
        return;
    }
    // Check if the client issuing the invite is a member of the channel
    if (!channel->get_client(client->get_nickname())) 
    {
        client->reply(ERR_NOTONCHANNEL(client->get_nickname(), channelName));
        return;
    }
    // Check if the channel is invite-only and if the client has admin privileges
    if (channel->is_invite_only() && !channel->get_admin(client->get_nickname())) {
        client->reply(ERR_CHANOPRIVSNEEDED(client->get_nickname(), channelName));
        return;
    }
    // Check if the target client is already in the channel
    if (channel->get_client(targetClient->get_nickname())) 
    {
        client->reply(ERR_USERONCHANNEL(targetNickname, channelName));
        return;
    }
    // Add the target client to the channel and notify all channel members of the invitation
    targetClient->join(channel);
    channel->broadcast(client->get_nickname() + " INVITE " + targetNickname + " :" + channelName);
}
