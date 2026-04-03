#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Matching_Engine/OrderBook.hpp"
#include <variant>


struct NewOrderCommand
{
    Order order;
};

struct CancelOrderCommand
{
    Symbol symbol;
    OrderId orderId;
};

struct ModifyOrderCommand
{
    Symbol symbol;
    OrderId orderId;
    Price price;
    Quantity quantity;
};

struct PrintCommand
{
    Symbol symbol;
};

struct PrintAllCommand
{
};

using Command = std::variant< NewOrderCommand, CancelOrderCommand, ModifyOrderCommand, PrintCommand , PrintAllCommand>;

#endif