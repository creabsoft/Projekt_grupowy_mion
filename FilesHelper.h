#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include "Damping.h"

namespace fs = std::filesystem;

class FilesHelper {
public:
	static std::vector<std::string> getPathToFilesFromDirectory(std::string pathToDirectory);
	static void splitStringToDouble(const std::string & someString, char delimiter, std::vector<double> & output);
	static bool saveDampingToFile(Damping damping, std::string filename);
	static bool saveSingleBlocksToBinary(std::vector<SingleBlock> singleBlocks, std::string filename);
	static void splitFilenameByDelimiter(const std::string & filename, char delimiter, std::vector<std::string> & output);
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
	std::ofstream file(filename, std::fstream::app);

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