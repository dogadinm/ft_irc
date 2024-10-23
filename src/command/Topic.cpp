#include "../../include/command/Command.hpp"

Topic::Topic(Server* server) : Command(server) {}

Topic::~Topic() {}
 // TOPIC [<channel>] [<topic>]

void Topic::execute(Client* client, std::vector<std::string> args)
{
    // Check for parameters
    if (args.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "TOPIC"));
        return;
    }

    std::string channelName = args[0];
    Channel* channel = _server->get_channel(channelName);

    // Check if the channel exists
    if (!channel)
    {
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), channelName));
        return;
    }

    // Check if the client is on the channel
    if (!client->get_channel(channelName))
    {
        client->reply(ERR_NOTONCHANNEL(client->get_nickname(), channelName));
        return;
    }

    // Check if the client has permission to install the theme
    if (args.size() > 1)
    {
        // Check permissions to change topic (must be channel operator)
        if(channel->is_topic_restricted())
        {
            if (client != channel->get_admin(client->get_nickname()))
            {
                client->reply(ERR_CHANOPRIVSNEEDED(client->get_nickname(), channelName));
                return;
            } 
        }


        // Install a new theme
        std::string newTopic = args[1];
        channel->set_topic(newTopic);
        
        // Notify all channel members about a new topic
        channel->broadcast(":" + client->get_nickname() + " TOPIC " + channelName + " :" + newTopic);
    }
    else
    {
        // If no new topic is specified, send the current topic
        std::string currentTopic = channel->get_topic();
        if (currentTopic.empty())
            client->reply(RPL_NOTOPIC(channelName));
        else
            client->reply(RPL_TOPIC(channelName, currentTopic));
    }
}

