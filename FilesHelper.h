#pragma once

#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

static class FilesHelper {
public:
	static std::vector<std::string> getPathToFilesFromDirectory(std::string pathToDirectory);
};

std::vector<std::string> FilesHelper::getPathToFilesFromDirectory(std::string pathToDirectory) {
	std::vector<std::string> pathToFiles;
	for (const auto & entry : fs::directory_iterator(pathToDirectory))
		pathToFiles.push_back(entry.path().string());

	return pathToFiles;
}