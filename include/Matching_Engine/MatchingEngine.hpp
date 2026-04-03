#ifndef MATCHING_ENGINE_HPP
#define MATCHING_ENGINE_HPP

#include "OrderBook.hpp"
#include <memory>
#include <variant>
#include <unordered_map>
#include "Command.hpp"

class MatchingEngine
{
public:
    MatchingEngine() = default;
    ~MatchingEngine() = default;

    void addOrder(Order order);

    void cancelOrder(Symbol symbol, OrderId orderId);
    
    void modifyOrder(Symbol symbol,
                     OrderId orderId,
                     Price price,
                     Quantity qty);

    void printBook(Symbol symbol) const;
    void printAllBooks() const;

    void handleCommand(const Command& command);

private:
    void onTrade(const Trade& trade);
    OrderBook& getOrderBook(Symbol symbol);

  

    void handle(const NewOrderCommand&);
    void handle(const CancelOrderCommand&);
    void handle(const ModifyOrderCommand&);
    void handle(const PrintCommand&);
    void handle(const PrintAllCommand&);

private:

    std::unordered_map<
        Symbol,
        std::unique_ptr<OrderBook>> m_books;
};

#endif // MATCHING_ENGINE_HPP