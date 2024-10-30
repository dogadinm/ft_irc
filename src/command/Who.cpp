#include "../../include/command/Command.hpp"

Who::Who(Server* server) : Command(server) {}

Who::~Who() {}

//syntax: WHOIS <name>

void Who::execute(Client* client, std::vector<std::string> args) 
{
    std::string channelName;
    if (!args.empty()) {
        channelName = args[0];
    }

    // If a channel name is specified, get information about clients in that channel
    if (!channelName.empty()) {
        Channel* channel = _server->get_channel(channelName);
        if (channel) {
            std::vector<Client*> clients = channel->get_clients();
            for (std::vector<Client*>::iterator it = clients.begin();
                 it != clients.end(); ++it) {
                Client* target = *it;
                std::string op = target->get_admin_access() ? "@" : " ";
                // Construct the WHO reply with the user's information
                std::string userInfo = RPL_WHOREPLY(
                    client->get_nickname(),
                    channel->get_name(),
                    target->get_username(),
                    target->get_hostname(),
                    _server->get_server_name(),
                    target->get_nickname(),
                    op, // '@' if the user is an operator
                    "0", // Placeholder for hopcount
                    target->get_realname()
                );

                client->reply(userInfo);
            }
            client->reply(RPL_ENDOFWHO(client->get_nickname(), channel->get_name()));
        } else {
            client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), channelName));
        }
    }
    // If no specific channel is provided, return general info about all clients on the server
    else {
        std::map<int, Client *> clients = _server->get_clients();
        for (std::map<int, Client *>::iterator it = clients.begin();
             it != clients.end(); ++it) {
            Client* target = it->second;
            std::string op = target->get_admin_access() ? "@" : " ";
            // Construct WHO reply for each client on the server
            std::string userInfo = RPL_WHOREPLY(
                client->get_nickname(),
                "*",
                target->get_username(),
                target->get_hostname(),
                _server->get_server_name(),
                target->get_nickname(),
                op,
                "0", // Placeholder for hopcount
                target->get_realname()
            );
            client->reply(userInfo);
        }
        client->reply(RPL_ENDOFWHO(client->get_nickname(), "*"));
    }
}

