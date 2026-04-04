#include <gtest/gtest.h>
#include "Matching_Engine/OrderBook.hpp"
#include "Matching_Engine/MatchingEngine.hpp"

class OrderBookTest : public ::testing::Test
{
protected:
    Symbol symbol = 1;
    OrderBook book{symbol};
};

TEST(OrderBookTest, MultiSymbolIsolation)
{
    OrderBook book1{1};
    OrderBook book2{2};

    bool trade1 = false;
    bool trade2 = false;

    book1.setTradeCallback([&](const Trade&){ trade1 = true; });
    book2.setTradeCallback([&](const Trade&){ trade2 = true; });

    Order buy{1,100,1,Side::BUY,100,10,0};
    Order sell{2,101,1,Side::SELL,100,10,0};

    book1.addOrder(buy);
    book2.addOrder(sell);

    EXPECT_FALSE(trade1);
    EXPECT_FALSE(trade2);
}

TEST_F(OrderBookTest, ModifyOrder)
{
    Order buy{1,100,1,Side::BUY,100,10,0};
    Order sell{1,101,1,Side::SELL,105,10,0};

    book.addOrder(buy);
    book.addOrder(sell);

    book.modifyOrder(100,105,10);

    bool trade = false;

    book.setTradeCallback(
        [&](const Trade&)
        {
            trade = true;
        });

    Order newSell{1,102,1,Side::SELL,105,10,0};
    book.addOrder(newSell);

    EXPECT_TRUE(trade);
}

TEST_F(OrderBookTest, CancelRemovesOrder)
{
    bool trade = false;

    book.setTradeCallback(
        [&](const Trade&)
        {
            trade = true;
        });

    Order buy{1,100,1,Side::BUY,100,10,0};
    Order sell{1,101,1,Side::SELL,100,10,0};

    book.addOrder(buy);

    book.cancelOrder(100);

    book.addOrder(sell);

    EXPECT_FALSE(trade);
}

TEST_F(OrderBookTest, AddBuyOrder)
{
    Order order{
        1,      // symbol
        100,    // orderId
        1,      // userId
        Side::BUY,
        100,    // price
        10,     // quantity
        0
    };

    book.addOrder(order);

    EXPECT_TRUE(true); // placeholder
}

TEST(MatchingEngineTest, BasicTrade)
{
    MatchingEngine engine;

    engine.handleCommand(
        NewOrderCommand{
            {1,100,1,Side::BUY,100,10,0}
        });

    engine.handleCommand(
        NewOrderCommand{
            {1,101,2,Side::SELL,100,10,0}
        });

    EXPECT_TRUE(true);
}

TEST(PositionEngineTest, BasicLongPosition)
{
    PositionEngine engine;

    Trade trade{
        100,101,
        1,2,
        100,
        10,
        1
    };

    engine.onFill(trade);

    EXPECT_TRUE(true);
}

TEST_F(OrderBookTest, PartialFill)
{
    int tradedQty = 0;

    book.setTradeCallback(
        [&](const Trade& trade)
        {
            tradedQty = trade.quantity;
        });

    Order buy{
        1, 100, 1, Side::BUY, 100, 20, 0
    };

    Order sell{
        1, 101, 1, Side::SELL, 100, 10, 0
    };

    book.addOrder(buy);
    book.addOrder(sell);

    EXPECT_EQ(tradedQty, 10);
}


TEST_F(OrderBookTest, PricePriority)
{
    int tradedPrice = 0;

    book.setTradeCallback(
        [&](const Trade& trade)
        {
            tradedPrice = trade.price;
        });

    Order sell1{1,101,1,Side::SELL,100,10,0};
    Order sell2{1,102,1,Side::SELL,99,10,0};

    Order buy{1,100,1,Side::BUY,100,10,0};

    book.addOrder(sell1);
    book.addOrder(sell2);

    book.addOrder(buy);

    EXPECT_EQ(tradedPrice, 99);
}
TEST_F(OrderBookTest, TimePriority)
{
    int firstTrade = 0;

    book.setTradeCallback(
        [&](const Trade& trade)
        {
            firstTrade = trade.sellOrderId;
        });

    Order sell1{1,101,1,Side::SELL,100,10,0};
    Order sell2{1,102,1,Side::SELL,100,10,0};

    Order buy{1,100,1,Side::BUY,100,10,0};

    book.addOrder(sell1);
    book.addOrder(sell2);

    book.addOrder(buy);

    EXPECT_EQ(firstTrade, 101);
}

TEST(IntegrationTest, FullFlow)
{
    MatchingEngine engine;

    engine.handleCommand(
        NewOrderCommand{
            {1,100,1,Side::BUY,100,10,0}
        });

    engine.handleCommand(
        NewOrderCommand{
            {1,101,2,Side::SELL,100,10,0}
        });

    engine.handleCommand(
        PositionCommand{1,1}
    );

    EXPECT_TRUE(true);
}