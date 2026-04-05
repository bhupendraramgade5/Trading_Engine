#include "Position_Engine/PositionEngine.hpp"

#include <iomanip>
#include <cmath>

Position& PositionEngine::getPosition(UserId user, Symbol symbol)
{
    return m_positions[user][symbol];
}

void PositionEngine::onFill(const Trade& trade)
{
    // Buyer
    {
        auto& pos = getPosition(trade.buyer, trade.symbol);

        int newQty = pos.netQty + trade.quantity;

        if (pos.netQty == 0)
        {
            pos.avgPrice = trade.price;
        }
        else if (pos.netQty > 0)
        {
            // Increasing long
            pos.avgPrice =
                ((pos.avgPrice * pos.netQty) +
                 (trade.price * trade.quantity)) / newQty;
        }
        else
        {
            // Closing short
            int closingQty = std::min(trade.quantity, -pos.netQty);

            pos.realizedPnL +=
                (pos.avgPrice - trade.price) * closingQty;

            if (newQty > 0)
                pos.avgPrice = trade.price;
        }

        pos.netQty = newQty;
        pos.lastPrice = trade.price;
        // Update unrealized
        pos.unrealizedPnL =
            (pos.lastPrice - pos.avgPrice) * pos.netQty;
    }

    // Seller
    {
        auto& pos = getPosition(trade.seller, trade.symbol);

        int newQty = pos.netQty - trade.quantity;

        if (pos.netQty == 0)
        {
            pos.avgPrice = trade.price;
        }
        else if (pos.netQty < 0)
        {
            // Increasing short
            pos.avgPrice =
                ((pos.avgPrice * std::abs(pos.netQty)) +
                 (trade.price * trade.quantity)) /
                std::abs(newQty);
        }
        else
        {
            // Closing long
            int closingQty = std::min(trade.quantity, pos.netQty);

            pos.realizedPnL +=
                (trade.price - pos.avgPrice) * closingQty;

            if (newQty < 0)
                pos.avgPrice = trade.price;
        }

        pos.netQty = newQty;
        pos.lastPrice = trade.price;
        pos.unrealizedPnL =
            (pos.lastPrice - pos.avgPrice) * pos.netQty;
    }
}

void PositionEngine::onPrice(Symbol symbol, Price price)
{
    for (auto& [user, symbolMap] : m_positions)
    {
        auto it = symbolMap.find(symbol);
        if (it == symbolMap.end())
            continue;

        auto& pos = it->second;

        pos.lastPrice = price;

        pos.unrealizedPnL =
            (pos.lastPrice - pos.avgPrice) * pos.netQty;
    }
}

void PositionEngine::print(UserId user, Symbol symbol) const
{
    auto userIt = m_positions.find(user);
    if (userIt == m_positions.end())
    {
        std::cout << "No position\n";
        return;
    }

    const auto& symbolMap = userIt->second;

    auto symbolIt = symbolMap.find(symbol);
    if (symbolIt == symbolMap.end())
    {
        std::cout << "No position\n";
        return;
    }

    const auto& pos = symbolIt->second;

    std::cout << "\n====== POSITION ======\n";

    std::cout << "User: " << user << "\n";
    std::cout << "Symbol: " << symbol << "\n";

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "NetQty: " << pos.netQty << "\n";
    std::cout << "AvgPrice: " << pos.avgPrice << "\n";
    std::cout << "RealizedPnL: " << pos.realizedPnL << "\n";
    std::cout << "UnrealizedPnL: " << pos.unrealizedPnL << "\n";

    std::cout << "======================\n";
}