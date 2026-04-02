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

enum class Side
{
    BUY,
    SELL
};

struct Order
{
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
    std::deque<Order>::iterator iterator;
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

    // Core API

    void addOrder(const Order& order);

    void cancelOrder(OrderId orderId);

    void modifyOrder(OrderId orderId, Price newPrice, Quantity newQty);

    void printBook() const;
public:

    using TradeCallback = std::function<void(const Trade&)>;

    void setTradeCallback(TradeCallback cb);

private:

    // Matching Logic
    void match();


private:

    // BUY SIDE
    std::map<Price, std::deque<Order>, std::greater<>> buySide;

    // SELL SIDE
    std::map<Price, std::deque<Order>> sellSide;

    // ORDER LOOKUP
    std::unordered_map<OrderId, OrderLocation> orderLookup;
};