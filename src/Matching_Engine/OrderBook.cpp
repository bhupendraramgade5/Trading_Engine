#include "OrderBook.hpp"

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <limits>
#include <variant>
#include <chrono>

using BuyBook  = std::map<Price, std::deque<Order>, std::greater<>>;
using SellBook = std::map<Price, std::deque<Order>>;

void OrderBook::addOrder(Order order)
{
    match(order);
    if (order.st_ordquantity > 0){
        if (order.st_ordside == Side::BUY)
            addBuyOrder(order);
        else
            addSellOrder(order);
    }
}

void OrderBook::addBuyOrder(Order order)
{
    auto& book = m_buySide;
    auto& queue = book[order.st_ordprice];
    queue.push_back(order);
    auto it = std::prev(queue.end());

    m_orderLookup[order.st_ordorderId] = {
        order.st_ordside,
        order.st_ordprice,
        it
    };
}

void OrderBook::addSellOrder(Order order)
{
    auto& book = m_sellSide;
    auto& queue = book[order.st_ordprice];
    queue.push_back(order);
    auto it = std::prev(queue.end());

    m_orderLookup[order.st_ordorderId] = {
        order.st_ordside,
        order.st_ordprice,
        it
    };
}


void OrderBook::match(Order& incoming)
{
    while (incoming.st_ordquantity > 0)
    {
        if (incoming.st_ordside == Side::BUY)
        {
            if (m_sellSide.empty())
                break;

            auto bestSell = m_sellSide.begin();

            if (bestSell->first > incoming.st_ordprice)
                break;

            auto& queue = bestSell->second;
            auto& topOrder = queue.front();

            Quantity traded =
                std::min(incoming.st_ordquantity, topOrder.st_ordquantity);

            incoming.st_ordquantity -= traded;
            topOrder.st_ordquantity -= traded;

            // generate trade
            Trade trade{
                incoming.st_ordorderId,
                topOrder.st_ordorderId,
                bestSell->first,
                traded
            };

            if (m_tradeCallback)
                TradeCallback(trade);

            if (topOrder.st_ordquantity == 0)
            {
                m_orderLookup.erase(topOrder.st_ordorderId);
                queue.pop_front();

                if (queue.empty())
                    m_sellSide.erase(bestSell);
            }
        }
        else
        {
            if (m_buySide.empty())
                break;

            auto bestBuy = m_buySide.begin();

            if (bestBuy->first < incoming.st_ordprice)
                break;

            auto& queue = bestBuy->second;
            auto& topOrder = queue.front();

            Quantity traded =
                std::min(incoming.st_ordquantity, topOrder.st_ordquantity);

            incoming.st_ordquantity -= traded;
            topOrder.st_ordquantity -= traded;

            Trade trade{
                topOrder.st_ordorderId,
                incoming.st_ordorderId,
                bestBuy->first,
                traded
            };

            if (m_tradeCallback)
                TradeCallback(trade);

            if (topOrder.st_ordquantity == 0)
            {
                m_orderLookup.erase(topOrder.st_ordorderId);
                queue.pop_front();

                if (queue.empty())
                    m_buySide.erase(bestBuy);
            }
        }
    }
}


void OrderBook::cancelOrder(OrderId orderId)
{
    auto it = m_orderLookup.find(orderId);
    if (it == m_orderLookup.end())
        return; // or throw

    auto& location = it->second;

    if (location.st_OrdLocside == Side::BUY)
    {
        auto bookIt = m_buySide.find(location.st_OrdLocprice);

        if (bookIt != m_buySide.end())
        {
            auto& queue = bookIt->second;
            queue.erase(location.st_ordIterator);

            if (queue.empty())
                m_buySide.erase(bookIt);
        }
    }
    else
    {
        auto bookIt = m_sellSide.find(location.st_OrdLocprice);

        if (bookIt != m_sellSide.end())
        {
            auto& queue = bookIt->second;
            queue.erase(location.st_ordIterator);

            if (queue.empty())
                m_sellSide.erase(bookIt);
        }
    }

    m_orderLookup.erase(it);
}

void OrderBook::modifyOrder(OrderId orderId, Price newPrice, Quantity newQty)
{
    auto it = m_orderLookup.find(orderId);
    if (it == m_orderLookup.end())
        return; // or throw

    auto location = it->second;
    auto& order = *(location.st_ordIterator);

    Order newOrder = order;

    newOrder.st_ordprice = newPrice;
    newOrder.st_ordquantity = newQty;
    newOrder.st_ordtimestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    cancelOrder(orderId);

    addOrder(newOrder);
}
