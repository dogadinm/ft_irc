#include "../../include/command/Command.hpp"

List::List(Server* server) : Command(server) {}

List::~List() {}

// syntax: KILL <nickname> :[<message>]

void List::execute(Client* client, std::vector<std::string> args) 
{
    // Notify that the LIST command has started
    if (!args.empty()){
        std::string channelName = args[0];
        std::cout << "hekllo012314" << std::endl;
        Channel* channel = _server->get_channel(channelName);
        std::cout << "hekllo05" << std::endl;
        if (channel)
        {
            std::cout << "hekllo0123" << std::endl;
            client->reply(RPL_LISTSTART(client->get_nickname()));
            // Count number of users in the channel
            std::stringstream ss;
            ss << channel->get_clients().size();
            std::string userCount = ss.str();
            // Get the topic of the channel
            std::string topic = channel->get_topic();
            // Show information about the specific channel
            client->reply(RPL_LIST(client->get_nickname(), channel->get_name(), userCount, topic));
            // Notify that the LIST command is complete
            client->reply(RPL_LISTEND(client->get_nickname(), _server->get_server_name()));
        }
        else
            client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), channelName));
            
    }
    else{
        client->reply(RPL_LISTSTART(client->get_nickname()));
        if(_server->get_channels().empty())
        {
            client->reply(RPL_LISTEND(client->get_nickname(), _server->get_server_name()));
            return;
        }
        // Loop through each channel on the server


        std::vector<Channel *> channles = _server->get_channels();
        for (std::vector<Channel *>::iterator it = channles.begin(); it != channles.end(); ++it) {
            Channel* channel = *it;
            // Count number of users in the channel
            std::stringstream ss;
            ss << channel->get_clients().size();
            std::string userCount = ss.str();
            // Get the topic of the channel
            std::string topic = channel->get_topic();
            // Send each channel info in response to the LIST command
            client->reply(RPL_LIST(client->get_nickname(), channel->get_name(), userCount, topic));
        }
        // Notify that the LIST command is complete
        client->reply(RPL_LISTEND(client->get_nickname(), _server->get_server_name()));
    }
}
