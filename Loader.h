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

	std::vector<SingleBlock> getBlocksFromFile(std::string filename);

};

Loader::Loader()
{
}

Loader::~Loader()
{
}

std::vector<SingleBlock> Loader::getBlocksFromFile(std::string filename) {
	FILE * file;
	errno_t err;
	struct SingleBlock singleBlock;
	std::vector<SingleBlock> blocksFromFile;

	if ((err = fopen_s(&file, filename.c_str(), "rb")) != 0) {
		std::cout << "File wasn't opened!" << std::endl;
	}
	else {
		while (fread(&singleBlock, sizeof(SingleBlock), 1, file)) {
			blocksFromFile.push_back(singleBlock);
			//printf("%d %d %d\n", singleBlock.time[0], singleBlock.before[0], singleBlock.after[0]);
			singleBlock = SingleBlock();
		}
		fclose(file);
	}

	return blocksFromFile;
}