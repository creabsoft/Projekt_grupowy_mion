#pragma once
#include <Vector>

const int MAX_BLOCKS_IN_STRUCTURE = 4;

struct SingleBlock {
	long time;
	long before;
	long after;

	SingleBlock() {}

	SingleBlock(long time, long before, long after) {
		this->time = time;
		this->before = before;
		this->after = after;
	}
};