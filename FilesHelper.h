#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include "Damping.h"
#include "BesselName.h"
#include "BesselNameHelper.h"

namespace fs = std::filesystem;
const std::string AFTER_DIR = "\\after\\";
const std::string BEFORE_DIR = "\\before\\";

class FilesHelper {
public:
	static std::vector<std::string> getPathToFilesFromDirectory(std::string pathToDirectory);
	static void splitStringToDouble(const std::string & someString, char delimiter, std::vector<double> & output);
	static bool saveDampingToFile(Damping damping, std::string filename);
	static bool saveSingleBlocksToBinary(std::vector<SingleBlock> singleBlocks, std::string filename);
	static void splitFilenameByDelimiter(const std::string & filename, char delimiter, std::vector<std::string> & output);
	static void saveToMatlabFormat(std::vector<SingleBlock> singleBlocks, std::string filename, std::string channel, std::string matlabFolder);
	static std::string getFilenameForDamping(std::string inputFilename);
	static void renameBesselFiles(std::string besselFolder, std::string fileWithNames);
	static std::vector<BesselName> getBesselsNamesFromFile(std::string filename);
};

std::vector<std::string> FilesHelper::getPathToFilesFromDirectory(std::string pathToDirectory) {
	std::vector<std::string> pathToFiles;
	for (const auto & entry : fs::directory_iterator(pathToDirectory))
		pathToFiles.push_back(entry.path().string());

	return pathToFiles;
}

void FilesHelper::splitStringToDouble(const std::string & someString, char delimiter, std::vector<double> &elems) {
	std::stringstream ss(someString);
	std::string item;
	while (std::getline(ss, item, delimiter)) {
		if (item.length() > 0) {
			elems.push_back(std::stod(item));
		}
	}
}

void FilesHelper::splitFilenameByDelimiter(const std::string & filename, char delimiter, std::vector<std::string> & output) {
	std::stringstream ss(filename);
	std::string item;
	while (std::getline(ss, item, delimiter)) {
		if (item.length() > 0) {
			output.push_back(item);
		}
	}
}

bool FilesHelper::saveDampingToFile(Damping damping, std::string filename) {
	std::ofstream file("dampings\\" + filename, std::fstream::app);

	if (file.is_open()) {
		file << std::to_string(damping.six60MHz) << " " << std::to_string(damping.two80MHz) << std::endl;
	}
	else {
		// return 1 to make information that something goes wrong
		return 0;
	}

	file.close();
	return 1;
}

bool FilesHelper::saveSingleBlocksToBinary(std::vector<SingleBlock> singleBlocks, std::string filename) {
	FILE * file;
	errno_t err;

	if ((err = fopen_s(&file, filename.c_str(), "wb")) != 0) {
		std::cout << "File wasn't opened!" << std::endl;
		
		// returns 0, because file wasn't opened!
		return 0;
	}
	else {
		for (SingleBlock singleBlock : singleBlocks) {
			fwrite(&singleBlock, sizeof(SingleBlock), 1, file);
		}
		fclose(file);
	}

	// returns 1 when everything works well
	return 1;
}

std::string FilesHelper::getFilenameForDamping(std::string inputFilename) {
	size_t found = inputFilename.find("ch1");
	if (found != std::string::npos) {
		return "ch1_dampings.txt";
	}

	found = inputFilename.find("ch2");
	if (found != std::string::npos) {
		return "ch2_dampings.txt";
	}

	found = inputFilename.find("ch3");
	if (found != std::string::npos) {
		return "ch3_dampings.txt";
	}
	else {
		return "";
	}
}

void FilesHelper::saveToMatlabFormat(std::vector<SingleBlock> singleBlocks, std::string filename, std::string channel, std::string matlabFolder) {
	std::ofstream outputBefore(matlabFolder + "\\" + channel + BEFORE_DIR + filename + "_BEFORE."+ channel + "_org");
	std::ofstream outputAfter(matlabFolder + "\\" + channel + AFTER_DIR + filename + "_AFTER." + channel + "_org");

	std::cout << "Saving before & after for: " << filename << std::endl;

	if (outputBefore.good() && outputAfter.good()) {
		int i = 0;
		for (i = 0; i < singleBlocks.size() - 1; i++) {
			outputBefore << singleBlocks[i].before << " ";
			outputAfter << singleBlocks[i].after << " ";
		}
		if (i < singleBlocks.size()) {
			outputBefore << singleBlocks[i].before;
			outputAfter << singleBlocks[i].after;
		}
	}

	outputBefore.close();
	outputAfter.close();
}

//void FilesHelper::renameBesselFiles(std::string besselFolder, std::string fileWithNames) {
//	std::vector<std::string> paths = FilesHelper::getPathToFilesFromDirectory(besselFolder);
//	std::vector<BesselName> besselsNames = getBesselsNamesFromFile(fileWithNames);
//	std::string lol;
//
//	for (auto it : paths) {
//		std::string filename = it.substr(it.find_last_of("\\") + 1);
//		std::string path = it.substr(0, it.find_last_of("\\") + 1);
//		std::vector<std::string> separatedFilename;
//		FilesHelper::splitFilenameByDelimiter(filename, '_', separatedFilename);
//
//		std::string newName = BesselNameHelper::getBesselNameForSelectedFile(besselsNames, separatedFilename[0]).newName;
//		std::cout << newName << std::endl;
//		std::cout << it << std::endl;
//	}
//}

std::vector<BesselName> FilesHelper::getBesselsNamesFromFile(std::string filename) {
	std::ifstream file(filename);
	std::vector<BesselName> besselsNames;

	if (file.is_open()) {
		std::string oldName;
		std::string newName;
		while (!file.eof()) {
			file >> oldName;
			file >> newName;

			besselsNames.push_back(BesselName(newName, oldName));
		}
	}

	return besselsNames;
}