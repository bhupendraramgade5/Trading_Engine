#ifndef POSITION_ENGINE_HPP
#define POSITION_ENGINE_HPP

#include "Matching_Engine/OrderBook.hpp"

struct Position
{
    int netQty = 0;
    double avgPrice = 0.0;
    double realizedPnL = 0.0;
    double unrealizedPnL = 0.0;
    double lastPrice = 0.0;

};


class PositionEngine
{
public:

    void onFill(const Trade& trade);

    void onPrice(Symbol symbol, Price price);

    void print(UserId user, Symbol symbol) const;

private:

    using SymbolPositions = std::unordered_map<Symbol, Position>;
    using UserPositions   = std::unordered_map<UserId, SymbolPositions>;

    UserPositions m_positions;

    Position& getPosition(UserId user, Symbol symbol);
};

#endif // POSITION_ENGINE_HPP