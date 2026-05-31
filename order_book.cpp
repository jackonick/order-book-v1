#include <iostream>
#include "order_book.h"
#include <algorithm>

void OrderBook::add_order(Order incoming) {
	if (incoming.side == Side::BUY) {
		while (incoming.size > 0 && !asks.empty() && asks.begin()->first
			<= incoming.price) {
			Order& resting = asks.begin()->second.front();
			uint64_t trade_size = std::min(incoming.size, resting.size);

			incoming.size -= trade_size;
			resting.size -= trade_size;

			std::cout << "TRADE: buy_id=" << incoming.id
				<< " sell_ID=" << resting.id
				<< " price=" << resting.price
				<< " size=" << trade_size << "\n";

			if (resting.size == 0) {
				asks.begin()->second.pop_front();
				if (asks.begin()->second.empty()) {
					asks.erase(asks.begin());
				}
			}
		}

		if (incoming.size > 0) {
			bids[incoming.price].push_back(incoming);
		}
	}


	else if (incoming.side == Side::SELL) {
		while (incoming.size > 0 && !bids.empty() && bids.begin()->first
			>= incoming.price) {
			Order& resting = bids.begin()->second.front();
			uint64_t trade_size = std::min(incoming.size, resting.size);

			incoming.size -= trade_size;
			resting.size -= trade_size;

			std::cout << "TRADE: sell_id=" << incoming.id
				<< " buy_ID=" << resting.id
				<< " price=" << resting.price
				<< " size=" << trade_size << "\n";

			if (resting.size == 0) {
				bids.begin()->second.pop_front();
				if (bids.begin()->second.empty()) {
					bids.erase(bids.begin());
				}
			}
		}

		if (incoming.size > 0) {
			asks[incoming.price].push_back(incoming);
		}
	}
}


void OrderBook::print() const {
	std::cout << "---ASKS---\n";
	for (const auto& [price, orders] : asks) {
		std::cout << " Price= " << price << "\n";
		for (const auto& order : orders) {
			std::cout << " Order_ID=" << order.id << " Order_Size=" << order.size << "\n";
		}
	}

	std::cout << "---BIDS---\n";
	for (const auto& [price, orders] : bids) {
		std::cout << " Price= " << price << "\n";
		for (const auto& order : orders) {
			std::cout << " Order_ID=" << order.id << " Order_Size=" << order.size << "\n";
		}
	}
}


int main() {
	OrderBook book;

	Order o1; o1.id = 1; o1.side = Side::SELL; o1.price = 100; o1.size = 10; o1.timestamp = 0;
	book.add_order(o1);

	Order o2; o2.id = 2; o2.side = Side::SELL; o2.price = 101; o2.size = 5; o2.timestamp = 1;
	book.add_order(o2);

	
	std::cout << "\n--- After two SELLs ---\n";
	book.print();

	
	Order o3; o3.id = 3; o3.side = Side::BUY; o3.price = 100; o3.size = 7; o3.timestamp = 2;
	book.add_order(o3);

	
	std::cout << "\n--- After BUY id=3 (should partial-fill id=1) ---\n";
	book.print();

	
	Order o4; o4.id = 4; o4.side = Side::BUY; o4.price = 99; o4.size = 5; o4.timestamp = 3;
	book.add_order(o4);

	
	std::cout << "\n--- After BUY id=4 (rests on bids) ---\n";
	book.print();

	Order o5; o5.id = 5; o5.side = Side::BUY; o5.price = 102; o5.size = 20; o5.timestamp = 4;
	book.add_order(o5);

	std::cout << "\n--- After BUY id=5 (walks both ask levels) ---\n";
	book.print();

	return 0;
}