#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
#include "SingleBlock.h"
#include "MatlabHelper.h"
#include "Loader.h"
#include "Bessel.h"
#include "BesselHelper.h"

const std::string OUTPUT_BINARY = "binary\\";
namespace fs = std::filesystem;

class Converter
{
public:
	Converter();
	~Converter();

	void separateAndSaveChannels(std::vector<std::string> pathToFiles);
	void convertBinaryToMatlab(std::vector<std::string> pathToFiles);
	void convertBesselToDamping(std::vector<std::string> pathToFiles);
	void convertFilesToCategories(std::vector<std::string> pathToFiles);

private:
	Loader loader;
};

Converter::Converter()
{
	if (!fs::exists("input")) {
		fs::create_directory("input");
	}
}


Converter::~Converter()
{
}

void Converter::separateAndSaveChannels(std::vector<std::string> pathToFiles) {
	int counter = 1;

	// creating directory / removing old
	if (fs::exists("binary")) {
		fs::remove_all("binary");
		fs::create_directory("binary");
	}
	else {
		fs::create_directory("binary");
	}

	for (auto it : pathToFiles) {
		std::cout << "Plik " << counter++ << " z " << pathToFiles.size() << std::endl;
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
					counter = 1;
				}
				else {
					counter++;
				}
			}
		} while (!file.eof());

		// saving the block into the file
		std::cout << "Saving into binary files in directory: " << OUTPUT_BINARY << std::endl;
		bool first = FilesHelper::saveSingleBlocksToBinary(ch0, OUTPUT_BINARY + filename + "_ch0.bin");
		bool second = FilesHelper::saveSingleBlocksToBinary(ch1, OUTPUT_BINARY + filename + "_ch1.bin");
		bool third = FilesHelper::saveSingleBlocksToBinary(ch2, OUTPUT_BINARY + filename + "_ch2.bin");
		if (first && second && third) {
			std::cout << "Finished saving into file!" << std::endl;
		}
		else {
			std::cout << "There was an error while saving the binary file..." << std::endl;
		}
	}
}

void Converter::convertBinaryToMatlab(std::vector<std::string> pathToFiles) {
	int counter = 1;

	// creating new structure of directories to be sure that there is no any file
	if (fs::exists("matlab")) {
		fs::remove_all("matlab");
	}
	fs::create_directory("matlab");
	fs::create_directory("matlab\\ch0");
	fs::create_directory("matlab\\ch1");
	fs::create_directory("matlab\\ch2");
	fs::create_directory("matlab\\ch0\\after");
	fs::create_directory("matlab\\ch1\\after");
	fs::create_directory("matlab\\ch2\\after");
	fs::create_directory("matlab\\ch0\\before");
	fs::create_directory("matlab\\ch1\\before");
	fs::create_directory("matlab\\ch2\\before");

	for (auto it : pathToFiles) {
		std::cout << "Plik " << counter++ << " z " << pathToFiles.size() << std::endl;
		std::vector<SingleBlock> channel = loader.getBlocksFromFile(it);
		std::string filename = it.substr(it.find_last_of("\\") + 1);
		filename = filename.substr(0, filename.size() - 4);
		std::string channelName = filename.substr(filename.find_last_of("_") + 1);
		MatlabHelper::saveToMatlabFormat(channel, filename, channelName);
	}
}

void Converter::convertBesselToDamping(std::vector<std::string> pathToFiles) {
	int counter = 1;
	std::ofstream file("dampings.txt");
	if (file.is_open()) {
		file.clear();
	}
	file.close();

	for (auto it : pathToFiles) {
		std::cout << "File " << counter++ << " from " << pathToFiles.size() << std::endl;
		std::vector<Bessel> besselsFromFile = loader.getBesselFromFile(it);
		Bessel maximumBessels = BesselHelper::getMaximumBessel(besselsFromFile);
		Damping damping = BesselHelper::calculateDampingFromBessel(maximumBessels);
		bool isCorrect = FilesHelper::saveDampingToFile(damping, "dampings.txt");
		if (!isCorrect) {
			std::cout << "There is an error while saving the damping into file!" << std::endl;
		}
		besselsFromFile.clear();
	}
}

void Converter::convertFilesToCategories(std::vector<std::string> pathToFiles) {
	int counter = 1;

	if (fs::exists("categories")) {
		fs::remove_all("categories");
	}

	fs::create_directory("categories");
	fs::create_directory("categories\\angles");
	fs::create_directory("categories\\enp");
	fs::create_directory("categories\\init_point");
	// std::stringstream sstr;

	const int ANGLE_INDEX = 1;
	const int ENP_INDEX = 2;
	const int INITPOINT_INDEX = 3;

	for (auto it : pathToFiles) {
		std::cout << "File " << counter++ << " from " << pathToFiles.size() << std::endl;
		std::vector<SingleBlock> blocksFromBinary = loader.getBlocksFromFile(it);
		std::string filename = it.substr(it.find_last_of("\\") + 1);
		std::vector<std::string> separatedFilename;
		FilesHelper::splitFilenameByDelimiter(filename, '_', separatedFilename);

		std::ifstream file(it);
		std::ofstream angleFile("categories\\angles\\" + separatedFilename[ANGLE_INDEX], std::fstream::app);
		std::ofstream enpFile("categories\\enp\\" + separatedFilename[ENP_INDEX], std::fstream::app);
		std::ofstream initpointFile("categories\\init_point\\" + separatedFilename[INITPOINT_INDEX], std::fstream::app);
		// while (file >> sstr.rdbuf());
		for (SingleBlock singleBlock : blocksFromBinary) {
			angleFile << singleBlock.time << " " << singleBlock.before << " " << singleBlock.after << std::endl;
			enpFile << singleBlock.time << " " << singleBlock.before << " " << singleBlock.after << std::endl;
			initpointFile << singleBlock.time << " " << singleBlock.before << " " << singleBlock.after << std::endl;
		}
		//angleFile << sstr.str();
		//enpFile << sstr.str();
		//initpointFile << sstr.str();
		//sstr.clear();
		angleFile << std::endl;
		enpFile << std::endl;
		initpointFile << std::endl;

		file.close();
		angleFile.close();
		enpFile.close();
		initpointFile.close();
	}
}