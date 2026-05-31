#pragma once
#include <cstdint>
#include <map>
#include <deque>



enum class Side {
	BUY,
	SELL
};

struct Order {
	Side side;
	uint64_t price;
	uint64_t timestamp;
	uint64_t id;
	uint64_t size;
};


class OrderBook {
public:
	void add_order(Order incoming);
	void print() const;

private:
	std::map<uint64_t, std::deque<Order>, std::greater<uint64_t>> bids;
	std::map<uint64_t, std::deque<Order>> asks;
};