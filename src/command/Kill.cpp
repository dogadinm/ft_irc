#include "../../include/command/Command.hpp"

Kill::Kill(Server* server) : Command(server) {}

Kill::~Kill() {}

// syntax: KILL <nickname> :[<message>]

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
    if (args.size() >= 3 && args[2][0] == ':')
    {
        reason = args[2].substr(1); // remove leading ':'
        for (size_t i = 3; i < args.size(); i++)  // append remaining words
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
    _server->client_disconnect(dest->get_fd());
}