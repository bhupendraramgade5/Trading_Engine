#include "OrderBook.hpp"

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <limits>
#include <variant>
#include <chrono>
#include <iomanip>

using BuyBook  = std::map<Price, std::deque<Order>, std::greater<>>;
using SellBook = std::map<Price, std::deque<Order>>;

void OrderBook::addOrder(Order order)
{
    match(order);
    if (order.quantity > 0){
        if (order.side == Side::BUY)
            addBuyOrder(order);
        else
            addSellOrder(order);
    }
}

void OrderBook::addBuyOrder(Order order)
{
    auto& book = m_buySide;
    auto& queue = book[order.price];
    queue.push_back(order);
    auto it = std::prev(queue.end());

    m_orderLookup[order.orderId] = {
        order.side,
        order.price,
        it
    };
}

void OrderBook::addSellOrder(Order order)
{
    auto& book = m_sellSide;
    auto& queue = book[order.price];
    queue.push_back(order);
    auto it = std::prev(queue.end());

    m_orderLookup[order.orderId] = {
        order.side,
        order.price,
        it
    };
}


void OrderBook::match(Order& incoming)
{
    while (incoming.quantity > 0)
    {
        if (incoming.side == Side::BUY)
        {
            if (m_sellSide.empty())
                break;

            auto bestSell = m_sellSide.begin();

            if (bestSell->first > incoming.price)
                break;

            auto& queue = bestSell->second;
            auto& topOrder = queue.front();

            Quantity traded =
                std::min(incoming.quantity, topOrder.quantity);

            incoming.quantity -= traded;
            topOrder.quantity -= traded;

            // generate trade
            Trade trade{
                incoming.orderId,
                topOrder.orderId,
                bestSell->first,
                traded
            };

            if (m_tradeCallback)
                TradeCallback(trade);

            if (topOrder.quantity == 0)
            {
                m_orderLookup.erase(topOrder.orderId);
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

            if (bestBuy->first < incoming.price)
                break;

            auto& queue = bestBuy->second;
            auto& topOrder = queue.front();

            Quantity traded =
                std::min(incoming.quantity, topOrder.quantity);

            incoming.quantity -= traded;
            topOrder.quantity -= traded;

            Trade trade{
                topOrder.orderId,
                incoming.orderId,
                bestBuy->first,
                traded
            };

            if (m_tradeCallback)
                TradeCallback(trade);

            if (topOrder.quantity == 0)
            {
                m_orderLookup.erase(topOrder.orderId);
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
        return;

    auto location = it->second;

    if (location.side == Side::BUY)
    {
        auto bookIt = m_buySide.find(location.price);

        if (bookIt != m_buySide.end())
        {
            auto& queue = bookIt->second;
            queue.erase(location.Iterator);

            if (queue.empty())
                m_buySide.erase(bookIt);
        }
    }
    else
    {
        auto bookIt = m_sellSide.find(location.price);

        if (bookIt != m_sellSide.end())
        {
            auto& queue = bookIt->second;
            queue.erase(location.Iterator);

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
    auto& order = *(location.Iterator);

    Order newOrder = order;

    newOrder.price = newPrice;
    newOrder.quantity = newQty;
    // newOrder.timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    auto now = std::chrono::steady_clock::now();
    newOrder.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

    cancelOrder(orderId);

    addOrder(newOrder);
}

void OrderBook::printBook() const
{
    std::cout << "\n========== ORDER BOOK ==========\n";

    std::cout << "\n------ SELL SIDE ------\n";
    std::cout << std::setw(10) << "Price"
              << std::setw(10) << "Qty" << "\n";

    for (const auto& [price, orders] : m_sellSide)
    {
        Quantity total = 0;

        for (const auto& order : orders)
            total += order.quantity;

        std::cout << std::setw(10) << price
                  << std::setw(10) << total << "\n";
    }

    std::cout << "\n-------------------------------\n";

    std::cout << "\n------ BUY SIDE ------\n";
    std::cout << std::setw(10) << "Price"
              << std::setw(10) << "Qty" << "\n";

    for (const auto& [price, orders] : m_buySide)
    {
        Quantity total = 0;

        for (const auto& order : orders)
            total += order.quantity;

        std::cout << std::setw(10) << price
                  << std::setw(10) << total << "\n";
    }

    std::cout << "\n===============================\n";
}