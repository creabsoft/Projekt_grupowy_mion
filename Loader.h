#pragma once

#include <vector>
#include <iostream>
#include <stdio.h>
#include "SingleBlock.h"

class Loader
{
public:
	Loader();
	~Loader();

	std::vector<std::vector<SingleBlock>> getBlocksFromFile(std::string filename);

};

Loader::Loader()
{
}


Loader::~Loader()
{
}


std::vector<std::vector<SingleBlock>> Loader::getBlocksFromFile(std::string filename) {
	FILE * file;
	errno_t err;
	struct SingleBlock singleBlock;
	std::vector<std::vector<SingleBlock>> blocks;
	std::vector<SingleBlock> fourBlocks;
	int counter = 1;
	sizeof(SingleBlock);
	if ((err = fopen_s(&file, filename.c_str(), "rb")) != 0) {
		std::cout << "File wasn't opened!" << std::endl;
	}
	else {
		while (fread(&singleBlock, sizeof(SingleBlock), 1, file)) {
			if (counter <= MAX_BLOCKS_IN_STRUCTURE) {
				fourBlocks.push_back(singleBlock);
				counter++;
			} else {
				counter = 1;
				blocks.push_back(fourBlocks);
				//std::cout << fourBlocks.size() << std::endl;
				fourBlocks.clear();
			}
			//printf("%d %d %d\n", singleBlock.time[0], singleBlock.before[0], singleBlock.after[0]);
			singleBlock = SingleBlock();
		}
	}

	fclose(file);

	std::cout << blocks.size() << std::endl;

	return blocks;
}