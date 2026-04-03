#include "MatchingEngine.hpp"

void MatchingEngine::handleCommand(const Command& command)
{
    std::visit(
        [this](auto&& cmd)
        {
            handle(cmd);
        },
        command
    );
}

OrderBook& MatchingEngine::getOrderBook(Symbol symbol)
{
    auto it = m_books.find(symbol);

    if (it == m_books.end())
    {
        auto book = std::make_unique<OrderBook>(symbol);

        // set trade callback
        book->setTradeCallback(
            [this](const Trade& trade)
            {
                onTrade(trade);
            });

        auto* ptr = book.get();

        m_books.emplace(symbol, std::move(book));

        return *ptr;
    }

    return *(it->second);
}

void MatchingEngine::cancelOrder(Symbol symbol, OrderId orderId)
{
    auto it = m_books.find(symbol);

    if (it == m_books.end())
        return;

    it->second->cancelOrder(orderId);
}


void MatchingEngine::addOrder(Order order)
{
    auto& book = getOrderBook(order.symbol);
    book.addOrder(std::move(order));
}

void MatchingEngine::modifyOrder(Symbol symbol,
                                 OrderId orderId,
                                 Price price,
                                 Quantity qty)
{
    auto it = m_books.find(symbol);

    if (it == m_books.end())
        return;

    it->second->modifyOrder(orderId, price, qty);
}

void MatchingEngine::printBook(Symbol symbol) const
{
    auto it = m_books.find(symbol);

    if (it == m_books.end())
    {
        std::cout << "No OrderBook for symbol: "
                  << symbol << "\n";
        return;
    }

    it->second->printBook();
}

void MatchingEngine::printAllBooks() const
{
    for (const auto& [symbol, book] : m_books)
    {
        std::cout << "\nSymbol: " << symbol << "\n";
        book->printBook();
    }
}

void MatchingEngine::onTrade(const Trade& trade)
{
    std::cout << "TRADE: "
              << trade.buyOrderId
              << " "
              << trade.sellOrderId
              << " "
              << trade.price
              << " "
              << trade.quantity
              << "\n";
}