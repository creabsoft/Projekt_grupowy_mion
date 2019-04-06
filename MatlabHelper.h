#pragma once

#include <vector>
#include <fstream>
#include "SingleBlock.h"

class MatlabHelper {
public:
	static void saveToMatlabFormat(std::vector<SingleBlock> singleBlocks, std::string filename);

};

void MatlabHelper::saveToMatlabFormat(std::vector<SingleBlock> singleBlocks, std::string filename) {
	std::ofstream outputBefore(filename + "_BEFORE.txt");
	std::ofstream outputAfter(filename + "_AFTER.txt");

	std::cout << "Saving before & after for: " << filename << std::endl;

	if (outputBefore.good() && outputAfter.good()) {
		for (SingleBlock singleBlock : singleBlocks) {
			outputBefore << singleBlock.before << " ";
			outputAfter << singleBlock.after << " ";
		}
	}

	outputBefore.close();
	outputAfter.close();
}