#pragma once
const int MAX_NUMBER_OF_BLOCKS = 760;

struct SingleBlock {
	long time[MAX_NUMBER_OF_BLOCKS];
	float before[MAX_NUMBER_OF_BLOCKS];
	float after[MAX_NUMBER_OF_BLOCKS];
};