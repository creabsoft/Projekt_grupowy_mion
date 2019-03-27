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

		do {
			getline(file, singleLine);

			if (numOfBlock < MAX_NUMBER_OF_BLOCKS && singleLine != "") {
				//  to separate the values from getline, used stringstream
				std::stringstream stringToSeparate(singleLine);
				stringToSeparate >> time >> before >> after;

				// assigning to the struct the values from file
				temporaryBlock.time[numOfBlock] = time;
				temporaryBlock.before[numOfBlock] = before;
				temporaryBlock.after[numOfBlock] = after;

				numOfBlock++;
			}
			// if singleLine is empty, saving the block and clearing the variables to be ready for next block
			else if (singleLine == "" && numOfBlock != 0) {
				singleBlocks.push_back(temporaryBlock);
				numOfBlock = 0;
				temporaryBlock = SingleBlock();
			}
		} while (!file.eof());

		// saving the block into the file
		saveToFile(singleBlocks, filename);

		// helpful to see how many blocks were in the file
		std::cout << singleBlocks.size() << std::endl;
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