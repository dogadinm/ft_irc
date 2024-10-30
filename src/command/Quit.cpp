

#include "../../include/command/Command.hpp"

Quit::Quit(Server* server, bool auth) : Command(server, auth) {}

Quit::~Quit() {}

// syntax: QUIT [<message>]

void    Quit::execute(Client* client, std::vector<std::string> args)
{
    std::string reason = args.empty() ? "Leaving..." : args.at(0);

    if (reason.at(0) == ':')
        reason = reason.substr(1);
    client->reply(RPL_QUIT(client->get_prefix(), reason));
    std::vector<Channel*> channels = client->get_channels();
	_server->client_disconnect(client->get_fd());
    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        Channel* channel = *it;
        if (channel->get_size() == 0)
        {
            log(channel->get_name() + " closed");
            _server->remove_channel(channel);
        }
            
    }
}
