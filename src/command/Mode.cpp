#include "../../include/command/Command.hpp"

Mode::Mode(Server* server) : Command(server) {}

Mode::~Mode() {}

// syntax:  MODE <nickname> <flags> (user)
//          MODE <channel> <flags> [<args>]

void Mode::execute(Client* client, std::vector<std::string> args)
{
    // handling errors
    if (args.size() < 2)
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "MODE"));
        return;
    }

    std::string target = args.at(0);

    Channel *channel = _server->get_channel(target); // MODE on clients not implemented
    if (!channel)
    {
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), target));
        return;
    }

    if (channel->get_admin(client->get_nickname()) != client)
    {
        client->reply(ERR_CHANOPRIVSNEEDED(client->get_nickname(), target));
        return;
    }

    // changing the mode
    int i = 0, p = 2;
    char c;

    while ((c = args[1][i]))
    {
        char prev_c = i > 0 ? args[1][i - 1] : '\0';
        bool active = prev_c == '+';

        switch (c)
        {
            case 'n': // External messages flag
            {
                channel->set_ext_msg(active);
                channel->broadcast(RPL_MODE(client->get_prefix(), channel->get_name(), (active ? "+n" : "-n"), ""));
                break;
            }
            case 'i': // Invite-only flag
            {
                channel->set_invite_only(active);
                channel->broadcast(RPL_MODE(client->get_prefix(), channel->get_name(), (active ? "+i" : "-i"), ""));
                break;
            }
            case 't': // Topic restrictions flag
            {
                channel->set_topic_restricted(active);
                channel->broadcast(RPL_MODE(client->get_prefix(), channel->get_name(), (active ? "+t" : "-t"), ""));
                break;
            }
            case 'l': // User limit flag
            {
                if (active)
                {
                    if (p < static_cast<int>(args.size()))
                    {
                        channel->set_limit(atoi(args[p].c_str()));
                        channel->broadcast(RPL_MODE(client->get_prefix(), channel->get_name(), "+l", args[p]));
                        p++;
                    }
                    else
                    {
                        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "MODE +l"));
                        return;
                    }
                }
                else
                {
                    channel->set_limit(0); // Remove limit
                    channel->broadcast(RPL_MODE(client->get_prefix(), channel->get_name(), "-l", ""));
                }
                break;
            }
            case 'k': // Channel key flag
            {
                if (active)
                {
                    if (p < static_cast<int>(args.size()))
                    {
                        channel->set_key(args[p]);
                        channel->broadcast(RPL_MODE(client->get_prefix(), channel->get_name(), "+k", args[p]));
                        p++;
                    }
                    else
                    {
                        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "MODE +k"));
                        return;
                    }
                }
                else
                {
                    channel->set_key(""); // Remove key
                    channel->broadcast(RPL_MODE(client->get_prefix(), channel->get_name(), "-k", ""));
                }
                break;
            }
            case 'o': // Operator privileges flag
            {
                if (p < static_cast<int>(args.size()))
                {
                    Client* target = _server->get_client(args[p]);
                    if (!target)
                    {
                        client->reply(ERR_NOSUCHNICK(client->get_nickname(), args[p]));
                        return;
                    }

                    if (active)
                    {
                        channel->add_operator(target);
                        channel->broadcast(RPL_MODE(client->get_prefix(), channel->get_name(), "+o", target->get_nickname()));
                    }
                    else
                    {
                        channel->remove_operator(target);
                        channel->broadcast(RPL_MODE(client->get_prefix(), channel->get_name(), "-o", target->get_nickname()));
                    }
                    p++;
                }
                else
                {
                    client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "MODE +o"));
                    return;
                }
                break;
            }
            default:
                break;
        }
        i++;
    }
}
