#include <gtest/gtest.h>
#include "Matching_Engine/OrderBook.hpp"

class OrderBookTest : public ::testing::Test
{
protected:
    Symbol symbol = 1;
    OrderBook book{symbol};
};

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


TEST_F(OrderBookTest, BasicMatch)
{
    bool tradeExecuted = false;

    book.setTradeCallback(
        [&](const Trade& trade)
        {
            tradeExecuted = true;
        });

    Order buy{
        1, 100, 1, Side::BUY, 100, 10, 0
    };

    Order sell{
        1, 101, 1, Side::SELL, 100, 10, 0
    };

    book.addOrder(buy);
    book.addOrder(sell);

    EXPECT_TRUE(tradeExecuted);
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

TEST_F(OrderBookTest, CancelOrder)
{
    Order buy{
        1, 100, 1, Side::BUY, 100, 10, 0
    };

    book.addOrder(buy);

    book.cancelOrder(100);

    EXPECT_TRUE(true);
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