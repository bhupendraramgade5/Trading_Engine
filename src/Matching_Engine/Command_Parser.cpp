#include "Matching_Engine/CommandParser.hpp"

#include <sstream>
#include <iostream>

std::optional<Command> CommandParser::parse(const std::string& input)
{
    std::istringstream iss(input);
    std::string commandType;

    if (!(iss >> commandType))
    {
        return std::nullopt;
    }

    if (commandType == "NEW")
    {
        return parseNew(iss);
    }
    else if (commandType == "CANCEL")
    {
        return parseCancel(iss);
    }
    else if (commandType == "MODIFY")
    {
        return parseModify(iss);
    }
    else if (commandType == "PRINT")
    {
        return parsePrint(iss);
    }
    else if (commandType == "PRINTALL")
    {
        return PrintAllCommand{};
    }
    else if (commandType == "PRICE")
    {
        return parsePrice(iss);
    }
    else if (commandType == "POSITION")
    {
        return parsePosition(iss);
    }

    return std::nullopt;
}


// Command CommandParser::parseNew(std::istringstream& iss)
// {
//     Symbol symbol;
//     OrderId orderId;
//     std::string sideStr;
//     Price price;
//     Quantity qty;

//     iss >> symbol >> orderId >> sideStr >> price >> qty;

//     Side side = (sideStr == "BUY") ? Side::BUY : Side::SELL;

//     Order order{
//         symbol,
//         orderId,
//         0, // userId (future)
//         side,
//         price,
//         qty,
//         0 // timestamp (set later)
//     };

//     return NewOrderCommand{order};
// }

Command CommandParser::parseNew(std::istringstream& iss)
{
    Symbol symbol;
    OrderId orderId;
    UserId userId;
    std::string sideStr;
    Price price;
    Quantity qty;

    iss >> symbol >> userId >> sideStr >> price >> qty;

    Side side = (sideStr == "BUY") ? Side::BUY : Side::SELL;

    Order order{
        symbol,
        orderId,
        userId,
        side,
        price,
        qty,
        0 // timestamp (set later)
    };

    return NewOrderCommand{order};
}

Command CommandParser::parseCancel(std::istringstream& iss)
{
    Symbol symbol;
    OrderId orderId;

    iss >> symbol >> orderId;

    return CancelOrderCommand{
        symbol,
        orderId
    };
}

Command CommandParser::parseModify(std::istringstream& iss)
{
    Symbol symbol;
    OrderId orderId;
    Price price;
    Quantity qty;

    iss >> symbol >> orderId >> price >> qty;

    return ModifyOrderCommand{
        symbol,
        orderId,
        price,
        qty
    };
}

Command CommandParser::parsePrint(std::istringstream& iss)
{
    Symbol symbol;

    iss >> symbol;

    return PrintCommand{
        symbol
    };
}

Command CommandParser::parsePrice(std::istringstream& iss)
{
    Symbol symbol;
    Price price;

    iss >> symbol >> price;

    return PriceCommand{
        symbol,
        price
    };
}

Command CommandParser::parsePosition(std::istringstream& iss)
{
    UserId user;
    Symbol symbol;

    iss >> user >> symbol;

    return PositionCommand{
        user,
        symbol
    };
}