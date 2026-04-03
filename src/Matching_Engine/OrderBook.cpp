#include "OrderBook.hpp"

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <limits>
#include <variant>

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