#pragma once
#include <Vector>

const int MAX_BLOCKS_IN_STRUCTURE = 4;

struct SingleBlock {
	int realSize;
	long * time;
	long * before;
	long * after;

	SingleBlock() {
		realSize = 0;
		time = new long[3000];
		before = new long[3000];
		after = new long[3000];
	}

	SingleBlock(int size) {
		realSize = size;
		time = new long[size];
		before = new long[size];
		after = new long[size];
	}

	~SingleBlock() {
		delete time; 
		delete before; 
		delete after;
	}
};