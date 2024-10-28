#include "../../include/command/Command.hpp"

WhoIs::WhoIs(Server* server) : Command(server) {}

WhoIs::~WhoIs() {}

//syntax: WHOIS <name>

void WhoIs::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "WHOIS"));
        return;
    }

    std::string targetNickname = args[0];
    Client* target = _server->get_client(targetNickname);

    if (!target)
    {
        client->reply(ERR_NOSUCHNICK(client->get_nickname(), targetNickname));
        return;
    }

    client->reply(RPL_WHOISUSER(client->get_nickname(), target->get_nickname(), target->get_username(),  target->get_hostname(), target->get_realname()));

    std::string channels;
    std::vector<Channel*> chnnls = target->get_channels();
    for (std::vector<Channel*>::iterator it = chnnls.begin(); it != chnnls.end(); ++it)
    {
        bool oper = target->get_channel((*it)->get_name())->get_admin(target->get_nickname());
        channels += oper ? "@" +  (*it)->get_name() + " " : "+" + (*it)->get_name() + " ";
    }
    client->reply(RPL_WHOISCHANNELS(client->get_nickname(), target->get_nickname(), channels));
    client->reply(RPL_WHOISSERVER(client->get_nickname(), target->get_nickname(), _server->get_server_name(), "no server info"));

    if(target->get_admin_access())
        client->reply(RPL_WHOISOPERATOR(client->get_nickname(), target->get_nickname()));

    std::stringstream ss;
    ss << target->get_idle_time();
    std::string idle = ss.str();
    client->reply(RPL_WHOISIDLE(client->get_nickname(), client->get_nickname(), idle));

    client->reply(RPL_ENDOFWHOIS(client->get_nickname(), target->get_nickname()));
}
