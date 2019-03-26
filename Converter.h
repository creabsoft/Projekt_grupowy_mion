#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

const int MAX_NUMBER_OF_BLOCKS = 760;

class Converter
{
public:
	Converter();
	~Converter();

	struct SingleBlock {
		int time;
		double before;
		double after;
	};

	std::vector<std::string> loadPathToFilesFromDirectory(std::string pathToDirectory);
	void convertToBinary(std::vector<std::string> pathToFiles);

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
	SingleBlock singleBlock[MAX_NUMBER_OF_BLOCKS];

	for (auto it : pathToFiles) {
		std::ifstream file;
		file.open(it);
		std::string singleLine;
		int i = 0;

		do {
			std::getline(file, singleLine);
			singleBlock[i]

		} while(singleLine != "\n")
	}
}