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