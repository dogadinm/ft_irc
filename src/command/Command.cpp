#include "../../include/command/Command.hpp"

Command::Command(Server* server, bool auth): _server(server), _auth(auth) {}

Command::~Command() {}

bool    Command::get_auth() const { return _auth; }