

#include "../../include/command/Command.hpp"

WhoIs::WhoIs(Server* server) : Command(server) {}

WhoIs::~WhoIs() {}

// syntax: WHO <name>

// void WhoIs::execute(Client* client, std::vector<std::string> args)
// {
//     // Проверяем, передан ли никнейм для whois
//     if (args.empty())
//     {
//         client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "WHOIS"));
//         return;
//     }

//     // Получаем целевой никнейм и проверяем его существование
//     std::string targetNickname = args[0];
//     Client* target = _server->get_client(targetNickname);

//     if (!target)
//     {
//         client->reply(ERR_NOSUCHNICK(client->get_nickname(), targetNickname));
//         return;
//     }

//     // Если пользователь найден, отправляем подробную информацию о нем

//     client->reply(RPL_WHOISUSER(client->get_nickname(), target->get_nickname(), target->get_username(),  target->get_hostname(), target->get_realname()));

//     // Выводим список каналов, в которых находится целевой пользователь
//     std::string channels;
//     for (std::vector<Channel*>::iterator it = target->get_channels().begin(); it != target->get_channels().end(); ++it)
//     {
//         channels += (*it)->get_name() + " ";
//     }
//     client->reply( " 319 " + client->get_nickname() + " " + 
//                  target->get_nickname() + " :" + channels);

//     // Выводим сервер и его адрес
//     client->reply( " 312 " + client->get_nickname() + " " + 
//                  target->get_nickname() + " " + _server->get_name() + " :Server info");

//     // Отмечаем, как долго целевой клиент неактивен
//     client->reply( " 317 " + client->get_nickname() + " " + 
//                  target->get_nickname() + " " + std::to_string(target->get_idle_time()) + 
//                  " :seconds idle");

//     // Завершаем ответ на команду WHOIS
//     client->reply( " 318 " + client->get_nickname() + " " + 
//                  target->get_nickname() + " :End of WHOIS list");
// }