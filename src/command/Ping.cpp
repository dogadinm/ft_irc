#include "../../include/command/Command.hpp"

Ping::Ping(Server* server) : Command(server) {}

Ping::~Ping() {}

// syntax: PING <server1> [<server2>]

void    Ping::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "PING"));
        return;
    }

    client->write(RPL_PING(client->get_prefix(), args.at(0)));
}
