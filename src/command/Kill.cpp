#include "../../include/command/Command.hpp"

Kill::Kill(Server* server) : Command(server) {}

Kill::~Kill() {}

// syntax: KILL <nickname> :[<message>]

void Kill::execute(Client* client, std::vector<std::string> args)
{
    std::cout << "hello0" << std::endl;
    std::cout << args[0] << std::endl;
    std::string target = args[0];
    std::string reason = "No reason specified!";
    std::cout << "hello01" << std::endl;
    if (args.size() < 2)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "KICK"));
        return;
    }
    std::cout << "hello" << std::endl;
    Client* dest = _server->get_client(target);
    std::cout << dest->get_username() << dest << std::endl;
    if (!dest)
    {
        client->reply(ERR_NOSUCHNICK(client->get_nickname(), target));
        return;
    }
    std::cout << "hello" << std::endl;
    if(client->get_admin_access() == true)
    {
        _server->client_disconnect(dest->get_fd());
    } 
    std::cout << "hello" << std::endl;
}