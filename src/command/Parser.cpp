#include "../../include/command/Parser.hpp"


Parser::Parser(server* srv): _srv(srv)
{
    
}

Parser::~Parser()
{}



std::string     Parser::trim(const std::string& str)
{
    size_t start = str.find_first_not_of("\t\n\r\f\v");
    // If string only \t\n\r\f\v
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of("\t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

void Parser::nvoke(Client* client, const std::string& message)
{}

