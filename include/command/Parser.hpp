#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <map>

#include "Command.hpp"

class Parser
{
    private:
        Server* _srv;
        std::map<std::string, Command *> _commands;

        std::string     trim(const std::string& str);
    public:
        Parser(server* srv);
        ~Parser();

        void invoke(Client* client, const std::string& message);
};