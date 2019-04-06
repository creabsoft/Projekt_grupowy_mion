#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
#include "SingleBlock.h"
#include "MatlabHelper.h"

const std::string OUTPUT_BINARY = "binary\\";
const std::string OUTPUT_BEFORE_AFTER = "matlab\\";

class Converter
{
public:
	Converter();
	~Converter();

	void separateAndSaveChannels(std::vector<std::string> pathToFiles);
	void saveToFile(std::vector<SingleBlock> singleBlocks, std::string filename);
};

Converter::Converter()
{
}


Converter::~Converter()
{
}

void Converter::separateAndSaveChannels(std::vector<std::string> pathToFiles) {
	for (auto it : pathToFiles) {
		std::vector<SingleBlock> ch0;
		std::vector<SingleBlock> ch1;
		std::vector<SingleBlock> ch2;
		std::ifstream file;
		file.open(it);

		// building the filename
		std::string filename = it.substr(it.find_last_of("\\") + 1);
		filename = filename.substr(0, filename.size() - 4);
		
		// helpful variables for iterating through file
		std::string singleLine;

		long time;
		float before, after;

		// counter to know which channel we are reading
		int counter = 1;

		do {
			getline(file, singleLine);

			if (singleLine != "" && counter < MAX_BLOCKS_IN_STRUCTURE) {
				//  to separate the values from getline, used stringstream
				std::stringstream stringToSeparate(singleLine);
				stringToSeparate >> time >> before >> after;
				SingleBlock block(time, before, after);
				if (counter == 1) {
					ch0.push_back(block);
				}
				else if (counter == 2) {
					ch1.push_back(block);
				}
				else if (counter == 3) {
					ch2.push_back(block);
				}
			}
			// if singleLine is empty, saving the block and clearing the variables to be ready for next block
			else if (singleLine == "") {
				if (counter == MAX_BLOCKS_IN_STRUCTURE) {
					counter = 0;
				}
				else {
					counter++;
				}
			}
		} while (!file.eof());

		// saving the block into the file
		std::cout << "Saving into binary files in directory: " << OUTPUT_BINARY << std::endl;
		saveToFile(ch0, OUTPUT_BINARY + filename + "_ch0.bin");
		saveToFile(ch1, OUTPUT_BINARY + filename + "_ch1.bin");
		saveToFile(ch2, OUTPUT_BINARY + filename + "_ch2.bin");
		std::cout << "Finished saving into binaries. \n\n";

		std::cout << "Saving into MatLab files in directory: " << OUTPUT_BEFORE_AFTER << std::endl;
		MatlabHelper::saveToMatlabFormat(ch0, OUTPUT_BEFORE_AFTER + filename + "_ch0");
		MatlabHelper::saveToMatlabFormat(ch1, OUTPUT_BEFORE_AFTER + filename + "_ch1");
		MatlabHelper::saveToMatlabFormat(ch2, OUTPUT_BEFORE_AFTER + filename + "_ch2");
		std::cout << "Finished saving into MatLab files. \n\n";
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
		fclose(file);
	}
}