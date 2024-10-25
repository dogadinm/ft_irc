#include "../../include/command/Command.hpp"

Invite::Invite(Server* server) : Command(server) {}

Invite::~Invite() {}

// INVITE <nick> [<channel>]

void Invite::execute(Client* client, std::vector<std::string> args) {
    if (args.size() < 2) {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "INVITE"));
        return;
    }

    std::string targetNickname = args[0];
    std::string channelName = args[1];
    Channel* channel = _server->get_channel(channelName);
    Client* targetClient = _server->get_client(targetNickname);

    if (!channel) 
    {
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), channelName));
        return;
    }
    
    if (!targetClient) 
    {
        client->reply(ERR_NOSUCHNICK(client->get_nickname(), targetNickname));
        return;
    }
    
    if (!channel->get_client(client->get_nickname())) 
    {
        client->reply(ERR_NOTONCHANNEL(client->get_nickname(), channelName));
        return;
    }
    
    if (channel->is_invite_only() && !channel->get_admin(client->get_nickname())) {
        client->reply(ERR_CHANOPRIVSNEEDED(client->get_nickname(), channelName));
        return;
    }

    if (channel->get_client(targetClient->get_nickname())) 
    {
        client->reply(ERR_USERONCHANNEL(targetNickname, channelName));
        return;
    }

    targetClient->join(channel);
    // client->reply(RPL_INVITING(client->get_nickname(), targetClient->get_nickname(), channelName));
    channel->broadcast(client->get_nickname() + " INVITE " + targetNickname + " :" + channelName);
}
