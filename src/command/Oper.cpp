#include "../../include/command/Command.hpp"

Oper::Oper(Server* server) : Command(server) {}

Oper::~Oper() {}

// syntax: Oper <name> <pass>

void    Oper::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "OPER"));
        return;
    }
    if(client->get_admin_access() == true)
    {
        client->reply(client->get_nickname() + "already oper");
        return;
    }

    // if (client->is_registered())
    // {
    //     client->reply(ERR_ALREADYREGISTERED(client->get_nickname()));
    //     return;
    // }
    // std::cout << args[1] << " " << args[1]<< std::endl;
    if (args[0] == _server->get_admin_name() && args[1] == _server->get_admin_pass())
    {
        client->set_admin_access(true);
        client->reply(client->get_nickname() + "You oper now");
    }
    // if (_server->get_Oper() != args[0].substr(args[0][0] == ':' ? 1 : 0))
    // {
    //     client->reply(ERR_OperWDMISMATCH(client->get_nickname()));
    //     return;
    // }

    // client->set_state(AUTHENTICATED);
}
