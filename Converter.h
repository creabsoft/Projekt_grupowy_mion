#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "SingleBlock.h"

namespace fs = std::filesystem;

class Converter
{
public:
	Converter();
	~Converter();

	std::vector<std::string> loadPathToFilesFromDirectory(std::string pathToDirectory);
	void convertToBinary(std::vector<std::string> pathToFiles);
	void saveToFile(std::vector<SingleBlock> singleBlocks, std::string filename);
	void optimizeBlocks(std::vector<SingleBlock> singleBlocks);

};

Converter::Converter()
{
}


Converter::~Converter()
{
}

std::vector<std::string> Converter::loadPathToFilesFromDirectory(std::string pathToDirectory) {
	std::vector<std::string> pathToFiles;
	for (const auto & entry : fs::directory_iterator(pathToDirectory))
		pathToFiles.push_back(entry.path().string());

	return pathToFiles;
}

void Converter::convertToBinary(std::vector<std::string> pathToFiles) {
	std::vector<SingleBlock> singleBlocks;

	for (auto it : pathToFiles) {
		SingleBlock temporaryBlock;
		std::ifstream file;
		file.open(it);

		// building the filename
		std::string filename = it.substr(it.find_last_of("/") + 1);
		filename = filename.substr(0, filename.size() - 4).append(".bin");
		
		// helpful variables for iterating through file
		std::string singleLine;
		int numOfBlock = 0;

		long time;
		float before, after;
		int i = 0;

		do {
			getline(file, singleLine);

			if (singleLine != "") {
				//  to separate the values from getline, used stringstream
				std::stringstream stringToSeparate(singleLine);
				stringToSeparate >> time >> before >> after;

				// assigning to the struct the values from file
				if (i < 3000) {
					temporaryBlock.time[i] = time;
					temporaryBlock.before[i] = before;
					temporaryBlock.after[i] = after;
					temporaryBlock.realSize++;
				}
			}
			// if singleLine is empty, saving the block and clearing the variables to be ready for next block
			else if (singleLine == "") {
				singleBlocks.push_back(temporaryBlock);
				temporaryBlock = SingleBlock();
			}
		} while (!file.eof());

		optimizeBlocks(singleBlocks);

		// saving the block into the file
		saveToFile(singleBlocks, filename);

		// helpful to see how many blocks were in the file
		//std::cout << singleBlocks.size() << std::endl;
	}
}

void Converter::optimizeBlocks(std::vector<SingleBlock> singleBlocks) {
	int i = 0;
	for (SingleBlock singleBlock : singleBlocks) {
		if (singleBlock.realSize < 3000) {
			SingleBlock withRealSize(singleBlock.realSize);
			for (int i = 0; i < singleBlock.realSize; i++) {
				withRealSize.time[i] = singleBlock.time[i];
				withRealSize.before[i] = singleBlock.before[i];
				withRealSize.after[i] = singleBlock.after[i];
			}
			singleBlocks.at(i) = withRealSize;
		}
		i++;
	}
}

void Converter::saveToFile(std::vector<SingleBlock> singleBlocks, std::string filename) {
	FILE * file;
	errno_t err;
	
	if ((err = fopen_s(&file, filename.c_str(), "wb")) != 0) {
		std::cout << "File wasn't opened!" << std::endl;
	}
	else {
		for (SingleBlock singleBlock : singleBlocks) {
			fwrite(&singleBlock, sizeof(SingleBlock), 1, file);
		}
	}

	fclose(file);
}

void 