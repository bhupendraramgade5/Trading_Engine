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
    Symbol st_ordsymbol;
    OrderId st_ordorderId;
    UserId st_orduserId;

    Side st_ordside;

    Price st_ordprice;
    Quantity st_ordquantity;

    uint64_t st_ordtimestamp;
};

struct OrderLocation
{
    Side st_OrdLocside;
    Price st_OrdLocprice;
    std::deque<Order>::iterator st_ordIterator;
};

struct Trade
{
    OrderId st_TrdbuyOrderId;
    OrderId st_TrdsellOrderId;

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

    std::function<void(const Trade&)> m_tradeCallback;
};

#endif // ORDERBOOK_HPP