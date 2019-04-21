#pragma once

#include <vector>
#include <fstream>
#include "SingleBlock.h"

const std::string MATLAB_DIR = "matlab\\";
const std::string AFTER_DIR = "\\after\\";
const std::string BEFORE_DIR = "\\before\\";

class MatlabHelper {
public:
	static void saveToMatlabFormat(std::vector<SingleBlock> singleBlocks, std::string filename, std::string channel);

};

void MatlabHelper::saveToMatlabFormat(std::vector<SingleBlock> singleBlocks, std::string filename, std::string channel) {
	std::ofstream outputBefore(MATLAB_DIR + channel + BEFORE_DIR + filename + "_BEFORE.txt");
	std::ofstream outputAfter(MATLAB_DIR + channel + AFTER_DIR + filename + "_AFTER.txt");

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