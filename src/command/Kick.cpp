// #include "../../include/command/Command.hpp"

// Kick::Kick(Server* srv) : Command(srv) {}

// Kick::~Kick() {}

// // syntax: KICK <channel> <client> :[<message>]

// void Kick::execute(Client* client, std::vector<std::string> args)
// {
//     if (args.size() < 2)
//     {
//         client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "KICK"));
//         return;
//     }

//     // extract channel name and target client
//     std::string channel_name = args[0];
//     std::string target = args[1];
//     std::string reason = "No reason specified!";

//     // if there is a reason provided (starts with ':')
//     if (args.size() >= 3 && args[2][0] == ':')
//     {
//         reason = args[2].substr(1); // remove leading ':'
//         for (size_t i = 3; i < args.size(); i++)  // append remaining words
//         {
//             reason.append(" " + args[i]);
//         }
//     }

//     // validate that the client is in the channel
//     Channel* channel = client->get_channel();
//     if (!channel || channel->get_name() != channel_name)
//     {
//         client->reply(ERR_NOTONCHANNEL(client->get_nickname(), channel_name));
//         return;
//     }

//     // check if client has administrative privileges
//     if (channel->get_admin() != client)
//     {
//         client->reply(ERR_CHANOPRIVSNEEDED(client->get_nickname(), channel_name));
//         return;
//     }

//     // check if the target client exists
//     Client* dest = _server->get_client(target);
//     if (!dest)
//     {
//         client->reply(ERR_NOSUCHNICK(client->get_nickname(), target));
//         return;
//     }

//     // check if the target client is in the same channel
//     if (!dest->get_channel() || dest->get_channel() != channel)
//     {
//         client->reply(ERR_USERNOTINCHANNEL(client->get_nickname(), dest->get_nickname(), channel_name));
//         return;
//     }

//     // if everything is valid, kick the target client
//     channel->kick(client, dest, reason);
// }
