#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <map>

class Parser;

#include "Command.hpp"

class Parser
{
    typedef std::map<std::string, Command *>::iterator comm_iter;

    private:
        Server* _srv;
        std::map<std::string, Command *> _commands;

        std::string     trim(const std::string& str);
    public:
        Parser(Server* srv);
        ~Parser();

        void invoke(Client* client, const std::string& message);
};