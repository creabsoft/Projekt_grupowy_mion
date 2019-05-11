#pragma once

#include <vector>
#include <iostream>
#include <stdio.h>
#include "FilesHelper.h"
#include "SingleBlock.h"
#include "Bessel.h"

class Loader
{
public:
	Loader();
	~Loader();

	std::vector<SingleBlock> getBlocksFromBinaryFile(std::string filename);
	std::vector<Bessel> getBesselFromFile(std::string filename);
};

Loader::Loader()
{
}

Loader::~Loader()
{
}

std::vector<SingleBlock> Loader::getBlocksFromBinaryFile(std::string filename) {
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
			singleBlock = SingleBlock();
		}
		fclose(file);
	}

	return blocksFromFile;
}

std::vector<Bessel> Loader::getBesselFromFile(std::string filename) {
	std::vector<Bessel> bessels;
	std::ifstream file(filename);

	std::vector<double> beforeSeparated;
	std::vector<double> six60MHzSeparated;
	std::vector<double> two80MHzSeparated;

	if (file.is_open()) {
		std::string trash;
		std::string before;
		std::string six60MHz;
		std::string two80MHz;
		getline(file, trash);
		getline(file, before);
		getline(file, six60MHz);
		getline(file, two80MHz);

		FilesHelper::splitStringToDouble(before, ' ', beforeSeparated);
		FilesHelper::splitStringToDouble(six60MHz, ' ', six60MHzSeparated);
		FilesHelper::splitStringToDouble(two80MHz, ' ', two80MHzSeparated);
	}

	for (int i = 0; i < beforeSeparated.size(); i++) {
		bessels.push_back(Bessel(beforeSeparated[i], six60MHzSeparated[i], two80MHzSeparated[i]));
	}

	return bessels;
}