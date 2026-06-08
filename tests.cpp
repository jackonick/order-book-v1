#include <gtest/gtest.h>
#include "order_book.h"

TEST(Suite, test){
    EXPECT_EQ(1, 1);
    EXPECT_TRUE(true);
}


TEST(RestsWhenNoMatch, AddOrder){
    OrderBook book;
    book.print();
    Order o1;
    o1.id = 5;
	o1.side = Side::BUY;
	o1.price = 102;
	o1.size = 20;
	o1.timestamp = 4;

    EXPECT_EQ(book.bid_levels(), 0);

    book.add_order(o1);

    EXPECT_EQ(book.bid_levels(), 1);

    book.print();
}