#include "../../include/command/Command.hpp"


User::User(Server* server, bool auth) : Command(server, auth) {}

User::~User() {}

// stntax: USER <username> <hostname> <servername> <realname>

void    User::execute(Client* client, std::vector<std::string> args)
{
    if (client->is_registered())
    {
        client->reply(ERR_ALREADYREGISTERED(client->get_nickname()));
        return;
    }

    if (args.size() < 4) 
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "USER"));
        return;
    }

    // if (client->get_state() == PASS)
    // {
    //     client->reply("Enter PASS and than NICK and USER");
    //     return;   
    // }
    // if (client->get_state() == UNAUTHENTICATED)
    // {
    //     client->reply("Enter NICK");
    //     return;   
    // }
    // if (client->get_state() == LOGIN)
    // {
    //     client->reply("Enter NICK");
    //     return;   
    // }




    // client->set_state(REGISTERED);
    client->set_username(args[0]);
    client->set_realname(args[3]);
    client->welcome();
}
