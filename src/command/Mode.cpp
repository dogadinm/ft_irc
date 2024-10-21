#include "../../include/command/Command.hpp"

Mode::Mode(Server* server) : Command(server) {}

Mode::~Mode() {}

// syntax:  MODE <nickname> <flags> (user)
//          MODE <channel> <flags> [<args>]

void Mode::execute(Client* client, std::vector<std::string> args)
{
    // Handling errors
    if (args.size() < 2)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "MODE"));
        return;
    }

    std::string target = args.at(0);

    Channel* channel = _server->get_channel(target);  // MODE on clients not implemented
    if (!channel)
    {
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), target));
        return;
    }

    if (channel->get_admin() != client)
    {
        client->reply(ERR_CHANOPRIVSNEEDED(client->get_nickname(), target));
        return;
    }

    // Changing the mode
    int i = 0, p = 2;
    char c;

    // while ((c = args[1][i]))
    // {
    //     char prev_c = i > 0 ? args[1][i - 1] : '\0';
    //     bool active = (prev_c == '+');  // Check if it's a + or - flag

    //     switch (c)
    //     {
    //         case 'n':  // No external messages mode
    //         {
}