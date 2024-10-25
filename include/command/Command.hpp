#pragma once

#include <numeric>
#include <string>
class Command;

#include "../network/Server.hpp"

class Command
{
    protected:
        Server* _server;
        bool _auth;

        Command();
        Command(Command const & copy);
    public:
        explicit Command(Server* server, bool auth = true);
        virtual ~Command();

        bool get_auth() const;

        virtual void execute(Client* client, std::vector<std::string> args) = 0;
};


// Command classes

class Notice : public Command
{
    public:
        Notice(Server* server);
        ~Notice();    
        void execute(Client* client, std::vector<std::string> args);
};

class Join : public Command
{
    public:
        Join(Server* server);
        ~Join();    
        void execute(Client* client, std::vector<std::string> args);
};

class Kick : public Command
{
    public:
        Kick(Server* server);
        ~Kick();    
        void execute(Client* client, std::vector<std::string> args);
};

class Mode : public Command
{
    public:
        Mode(Server* server);
        ~Mode();    
        void execute(Client* client, std::vector<std::string> args);
};

class Nick : public Command
{
    public:
        Nick(Server* server, bool auth);
        ~Nick();    
        void execute(Client* client, std::vector<std::string> args);
};


class Part : public Command
{
    public:
        Part(Server* server);
        ~Part();    
        void execute(Client* client, std::vector<std::string> args);
};

class Pass : public Command
{
    public:
        Pass(Server* server, bool auth);
        ~Pass();    
        void execute(Client* client, std::vector<std::string> args);
};

class Topic : public Command
{
    public:
        Topic(Server* server);
        ~Topic();    
        void execute(Client* client, std::vector<std::string> args);
};

class WhoIs : public Command
{
    public:
        WhoIs(Server* server);
        ~WhoIs();    
        void execute(Client* client, std::vector<std::string> args);
};


class Invite : public Command
{
    public:
        Invite(Server* server);
        ~Invite();    
        void execute(Client* client, std::vector<std::string> args);
};

class Names : public Command
{
    public:
        Names(Server* server);
        ~Names();    
        void execute(Client* client, std::vector<std::string> args);
};


class Ping : public Command
{
    public:
        Ping(Server* server);
        ~Ping();    
        void execute(Client* client, std::vector<std::string> args);
};

class Pong : public Command
{
    public:
        Pong(Server* server);
        ~Pong();    
        void execute(Client* client, std::vector<std::string> args);
};

class PrivMsg : public Command
{
    public:
        PrivMsg(Server* server);
        ~PrivMsg();    
        void execute(Client* client, std::vector<std::string> args);
};

class Kill : public Command
{
    public:
        Kill(Server* server);
        ~Kill();    
        void execute(Client* client, std::vector<std::string> args);
};

class Oper : public Command
{
    public:
        Oper(Server* server);
        ~Oper();    
        void execute(Client* client, std::vector<std::string> args);
};

class Quit : public Command
{
    public:
        Quit(Server* server, bool auth);
        ~Quit();    
        void execute(Client* client, std::vector<std::string> args);
};

class User : public Command
{
    public:
        User(Server* server, bool auth);
        ~User();    
        void execute(Client* client, std::vector<std::string> args);
};



