#include "../../include/command/Command.hpp"

Nick::Nick(Server* server, bool auth) : Command(server, auth) {}

Nick::~Nick(){}


void Nick::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty() || args[0].empty()) 
    {
        client->reply(ERR_NONICKNAMEGIVEN(client->get_nickname()));
        return;
    }
    if (client->get_state() == PASS)
    {
        client->reply("Enter PASS");
        return;   
    }

    std::string nickname = args[0];

    if(_server->get_client(nickname))
    {
        client->reply(ERR_NICKNAMEINUSE(client->get_nickname()));
        return;     
    }
    if (client->get_state() != REGISTERED )
        client->set_state(AUTHENTICATED);
    client->set_nickname(nickname);
    // client->welcome();
}