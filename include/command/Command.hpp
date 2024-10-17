#pragma once

#include <numeric>
#include <string>

#include "../network/Server.hpp"

class Command
{
    protected:
        Server* _srv;
        bool _auth;

        Command();
        Command(Command const & copy);
    public:
        explicit Command(Server* srv, bool auth = true);
        virtual ~Command();

        bool get_auth() const;

        virtual void execute(Client* client, std::vector<std::string> args) = 0;
};


