#include "../../include/command/Command.hpp"

Names::Names(Server* server) : Command(server) {}

Names::~Names() {}

// syntax: NAMES <channel> 

void Names::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty()) {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "NAMES"));
        return;
    }

    std::string channelName = args[0];
    Channel* channel = _server->get_channel(channelName);

    if (!channel) {
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), channelName));
        return;
    }
    std::string users = "";
    std::vector<std::string> nicknames = channel->get_nicknames();
    for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); ++it)
    {
        users.append(*it + " ");
    }

    client->reply(RPL_NAMREPLY(client->get_nickname() ,channel->get_name(), users));
    client->reply(RPL_ENDOFNAMES(client->get_nickname(), channel->get_name()));
}