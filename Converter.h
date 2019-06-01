#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
#include "SingleBlock.h"
#include "Loader.h"
#include "Bessel.h"
#include "BesselHelper.h"
#include "Histogram.h"
#include "HistogramHelper.h"

namespace fs = std::filesystem;

class Converter
{
public:
	Converter();
	~Converter();

	void separateAndSaveChannels(std::vector<std::string> pathToFiles, std::string outputFolder);
	void convertBinaryToMatlab(std::vector<std::string> pathToFiles, std::string matlabFolder);
	void convertBesselToDamping(std::vector<std::string> pathToFiles, std::string fileWithNames);
	void convertFilesToCategories(std::vector<std::string> pathToFiles);
	void generateHistogram(std::string filename, double rangeFrom, double rangeTo, int numOfCompartments);

private:
	Loader loader;
};

Converter::Converter()
{
	if (!fs::exists("input")) {
		fs::create_directory("input");
	}
	if (!fs::exists("bessel")) {
		fs::create_directory("bessel"); 
	}
}


Converter::~Converter()
{
}

void Converter::separateAndSaveChannels(std::vector<std::string> pathToFiles, std::string outputFolder) {
	int counter = 1;

	// creating directory / removing old
	if (fs::exists(outputFolder)) {
		fs::remove_all(outputFolder);
		fs::create_directory(outputFolder);
	}
	else {
		fs::create_directory(outputFolder);
	}

	for (auto it : pathToFiles) {
		std::cout << "File " << counter++ << " from " << pathToFiles.size() << std::endl;
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
		std::cout << "Saving into binary files in directory: " << outputFolder << std::endl;
		bool first = FilesHelper::saveSingleBlocksToBinary(ch0, outputFolder + "\\" + filename + "_ch0.bin");
		bool second = FilesHelper::saveSingleBlocksToBinary(ch1, outputFolder + "\\" + filename + "_ch1.bin");
		bool third = FilesHelper::saveSingleBlocksToBinary(ch2, outputFolder + "\\" + filename + "_ch2.bin");
		if (first && second && third) {
			std::cout << "Finished saving into file!" << std::endl;
		}
		else {
			std::cout << "There was an error while saving the binary file..." << std::endl;
		}
	}
}

void Converter::convertBinaryToMatlab(std::vector<std::string> pathToFiles, std::string matlabFolder) {
	int counter = 1;

	// creating new structure of directories to be sure that there is no any file
	if (fs::exists(matlabFolder)) {
		fs::remove_all(matlabFolder);
	}
	fs::create_directory(matlabFolder);
	fs::create_directory(matlabFolder + "\\ch0");
	fs::create_directory(matlabFolder + "\\ch1");
	fs::create_directory(matlabFolder + "\\ch2");
	fs::create_directory(matlabFolder + "\\ch0\\after");
	fs::create_directory(matlabFolder + "\\ch1\\after");
	fs::create_directory(matlabFolder + "\\ch2\\after");
	fs::create_directory(matlabFolder + "\\ch0\\before");
	fs::create_directory(matlabFolder + "\\ch1\\before");
	fs::create_directory(matlabFolder + "\\ch2\\before");

	for (auto it : pathToFiles) {
		std::cout << "File " << counter++ << " from " << pathToFiles.size() << std::endl;
		std::vector<SingleBlock> channel = loader.getBlocksFromBinaryFile(it);
		std::string filename = it.substr(it.find_last_of("\\") + 1);
		filename = filename.substr(0, filename.size() - 4);
		std::string channelName = filename.substr(filename.find_last_of("_") + 1);
		FilesHelper::saveToMatlabFormat(channel, filename, channelName, matlabFolder);
	}
}

void Converter::convertBesselToDamping(std::vector<std::string> pathToFiles, std::string fileWithNames) {
	int counter = 1;
	if (fs::exists("dampings")) {
		fs::remove_all("dampings");
	}
	fs::create_directory("dampings");
	fs::create_directory("dampings\\angle");
	fs::create_directory("dampings\\energy");
	fs::create_directory("dampings\\init_point");
	
	std::vector<Bessel> besselsFromFile;
	Bessel maximumBessels;
	Damping damping;
	std::vector<BesselName> besselsNames = FilesHelper::getBesselsNamesFromFile(fileWithNames);

	for (auto it : pathToFiles) {
		std::cout << "File " << counter++ << " from " << pathToFiles.size() << std::endl;
		besselsFromFile = loader.getBesselFromFile(it);
		std::string filename = it.substr(it.find_last_of("\\") + 1);
		std::vector<std::string> separatedFilename;
		FilesHelper::splitFilenameByDelimiter(filename, '_', separatedFilename);
		std::string newName = BesselNameHelper::getBesselNameForSelectedFile(besselsNames, separatedFilename[0]).newName;
		separatedFilename.clear();
		FilesHelper::splitFilenameByDelimiter(newName, '_', separatedFilename);
		// std::cout << "ANGLE: " << separatedFilename[2] << std::endl;

		std::ofstream angleFile("dampings\\angle\\" + separatedFilename[2], std::fstream::app);
		std::ofstream energyFile("dampings\\energy\\" + separatedFilename[3], std::fstream::app);
		std::ofstream initpointFile("dampings\\init_point\\" + separatedFilename[4], std::fstream::app);

		std::vector<Damping> dampings = BesselHelper::getDampingsFromBessels(besselsFromFile);

		for (auto damping : dampings) {
			angleFile << damping.six60MHz << " " << damping.two80MHz << " " << newName << std::endl;
			energyFile << damping.six60MHz << " " << damping.two80MHz << " " << newName << std::endl;
			initpointFile << damping.six60MHz << " " << damping.two80MHz << " " << newName << std::endl;
		}

		besselsFromFile.clear();
		angleFile.close();
		energyFile.close();
		initpointFile.close();
	}
}

void Converter::generateHistogram(std::string filename, double rangeFrom, double rangeTo, int numOfCompartments) {
	std::ifstream file(filename);

	std::vector<Damping> dampings;
	std::string trash;

	if (file.is_open()) {
		while (!file.eof()) {
			double six60MHz = 0.0;
			double two80MHz = 0.0;

			file >> six60MHz;
			file >> two80MHz;
			file >> trash;
			
			dampings.push_back(Damping(six60MHz, two80MHz));
		}
	}
	else {
		std::cout << "Cannot create histogram! Problem with dampings." << std::endl;
		return;
	}

	Histogram histogram(dampings, rangeFrom, rangeTo, numOfCompartments);
	histogram.generateSimpleHistograms();
	std::cout << "Successfully created histograms in files: histogramSix60MHz.txt and histogramTwo80MHz.txt (directory histograms)" << std::endl;
	std::cout << "The csv files you can find in the histograms folder." << std::endl;
}