#include <iostream>
#include <string>

#include "Matching_Engine/MatchingEngine.hpp"
#include "Matching_Engine/CommandParser.hpp"

int main()
{
    MatchingEngine engine;
    CommandParser parser;

    std::string line;

    std::cout << "Matching Engine Started\n";
    std::cout << "Enter commands:\n";

    while (std::getline(std::cin, line))
    {
        if (line.empty())
            continue;

        if (line == "EXIT" || line == "QUIT")
            break;

        auto command = parser.parse(line);

        if (!command)
        {
            std::cout << "Invalid command\n";
            continue;
        }

        engine.handleCommand(*command);
    }

    std::cout << "Matching Engine Stopped\n";

    return 0;
}