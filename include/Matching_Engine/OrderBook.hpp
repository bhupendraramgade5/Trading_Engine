#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include <iostream>
#include <map>
#include <deque>
#include <unordered_map>
#include <cstdint>
#include <functional>



using OrderId = uint64_t;
using Price = int;
using Quantity = int;
using UserId = uint64_t;
using Symbol = uint32_t;

enum class Side
{
    BUY,
    SELL
};

struct Order
{
    Symbol symbol;
    OrderId orderId;
    UserId userId;

    Side side;

    Price price;
    Quantity quantity;

    uint64_t timestamp;
};

struct OrderLocation
{
    Side side;
    Price price;
    std::deque<Order>::iterator Iterator;
};

struct Trade
{
    OrderId buyOrderId;
    OrderId sellOrderId;

    Price price;
    Quantity quantity;
};


class OrderBook
{
public:

    OrderBook() = default;
    ~OrderBook() = default;

    explicit OrderBook(Symbol symbol);

    // Core API

    void addOrder(Order order);

    void cancelOrder(OrderId orderId);

    void modifyOrder(OrderId orderId, Price newPrice, Quantity newQty);

    void printBook() const;
public:

    using TradeCallback = std::function<void(const Trade&)>;

    void setTradeCallback(TradeCallback cb);

private:

    // Matching Logic
    void  match(Order& incoming);
    void addBuyOrder(Order order);
    void addSellOrder(Order order);


private:

    // BUY SIDE
    std::map<Price, std::deque<Order>,std::greater<Price>> m_buySide;

    // SELL SIDE
    std::map<Price, std::deque<Order>> m_sellSide;

    // ORDER LOOKUP
    std::unordered_map<OrderId, OrderLocation> m_orderLookup;

    TradeCallback m_tradeCallback;

    Symbol m_symbol;
};

#endif // ORDERBOOK_HPP