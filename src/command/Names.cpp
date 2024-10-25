#include "../../include/command/Command.hpp"

Names::Names(Server* server) : Command(server) {}

Names::~Names() {}

// syntax: NAMES <channels> 

void Names::execute(Client* client, std::vector<std::string> args)
{
    // Проверяем, указан ли канал
    if (args.empty()) {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "NAMES"));
        return;
    }

    std::string channelName = args[0];
    Channel* channel = _server->get_channel(channelName);

    // Проверяем, существует ли канал
    if (!channel) {
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), channelName));
        return;
    }

    // Получаем список пользователей на канале
    // std::string usersList;
    // for (std::vector<Client*>::iterator it = channel->get_clients().begin(); it != channel->get_clients().end(); ++it) 
    // {
    //     std::cout << (*it)->get_nickname() << std::endl;
    //     usersList += (*it)->get_nickname() + " ";
    // }
    std::string users = "";
    std::vector<std::string> nicknames = channel->get_nicknames();
    for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); ++it)
    {
        users.append(*it + " ");
    }

    // Отправляем список пользователей
    client->reply(RPL_NAMREPLY(client->get_nickname() ,channel->get_name(), users));
    client->reply(RPL_ENDOFNAMES(client->get_nickname(), channel->get_name()));
}