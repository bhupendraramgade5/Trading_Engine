#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include "Matching_Engine/Command.hpp"

#include <string>
#include <optional>

class CommandParser
{
public:

    std::optional<Command> parse(const std::string& input);

private:

    Command parseNew(std::istringstream& iss);

    Command parseCancel(std::istringstream& iss);

    Command parseModify(std::istringstream& iss);

    Command parsePrint(std::istringstream& iss);

    Command parsePrice(std::istringstream& iss);

    Command parsePosition(std::istringstream& iss);


};

#endif // COMMAND_PARSER_HPP
