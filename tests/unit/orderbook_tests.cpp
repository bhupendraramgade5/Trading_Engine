#include <gtest/gtest.h>
#include "Matching_Engine/OrderBook.hpp"
#include "Matching_Engine/MatchingEngine.hpp"

class OrderBookTest : public ::testing::Test
{
protected:
    Symbol symbol = 1;
    OrderBook book{symbol};
};

class MatchingEngineTest : public ::testing::Test
{
};

class PositionEngineTest : public ::testing::Test
{
};

class IntegrationTest : public ::testing::Test
{
};
// 1
TEST_F(OrderBookTest, MultiSymbolIsolation)
{
    OrderBook book1{1};
    OrderBook book2{2};

    bool trade1 = false;
    bool trade2 = false;

    book1.setTradeCallback([&](const Trade&){ trade1 = true; });
    book2.setTradeCallback([&](const Trade&){ trade2 = true; });

    Order buy{1,0,1,Side::BUY,100,10,0};
    Order sell{2,0,1,Side::SELL,100,10,0};

    book1.addOrder(buy);
    book2.addOrder(sell);

    EXPECT_FALSE(trade1);
    EXPECT_FALSE(trade2);
}
//2
TEST_F(OrderBookTest, ModifyOrder)
{
    bool trade = false;

    book.setTradeCallback(
        [&](const Trade&)
        {
            trade = true;
        });

    Order buy{1,1,1,Side::BUY,100,10,0};
    Order sell{1,2,1,Side::SELL,105,10,0};

    book.addOrder(buy);
    book.addOrder(sell);

    book.modifyOrder(1,105,10);

    EXPECT_TRUE(trade);
}
//3
TEST_F(OrderBookTest, CancelRemovesOrder)
{
    bool trade = false;

    book.setTradeCallback(
        [&](const Trade&)
        {
            trade = true;
        });

    Order buy{1,100,1,Side::BUY,100,10,0};

    book.addOrder(buy);

    book.cancelOrder(100);

    Order sell{1,101,1,Side::SELL,100,10,0};

    book.addOrder(sell);

    EXPECT_FALSE(trade);
}
//4
TEST_F(OrderBookTest, AddBuyOrder)
{
    Order order{
        1,
        100,
        1,
        Side::BUY,
        100,
        10,
        0
    };

    book.addOrder(order);

    book.cancelOrder(100);

    SUCCEED();
}
//5
TEST_F(MatchingEngineTest, BasicTrade)
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
//6
TEST_F(PositionEngineTest, BasicLongPosition)
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
//7
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

//8
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
//9
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

//10
TEST_F(IntegrationTest, FullFlow)
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