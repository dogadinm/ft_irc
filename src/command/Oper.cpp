#include "../../include/command/Command.hpp"

Oper::Oper(Server* server) : Command(server) {}

Oper::~Oper() {}

// syntax: OPER <name> <pass>

void    Oper::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty() || args.size() < 2)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "OPER"));
        return;
    }
    if(client->get_admin_access() == true)
    {
        client->reply(RPL_YOUREOPER(client->get_nickname()));
        return;
    }
    if (args[1] != _server->get_admin_pass())
    {
        client->reply(ERR_PASSWDMISMATCH(client->get_nickname()));
        return;
    }
    if (args[0] == _server->get_admin_name())
    {
        client->set_admin_access(true);
        client->reply(RPL_YOUREOPER(client->get_nickname()));
    }
}
