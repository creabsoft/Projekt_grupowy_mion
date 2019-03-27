#include <iostream>
#include "Converter.h"
#include "Loader.h"

int main()
{
	Converter converter;
	Loader loader;
	
	//std::vector<std::string> filenames = loader.getFilenamesFromDirectory("test");

//	for (auto it : filenames) {
	//	std::cout << it << std::endl;
	//
	std::vector<std::string> paths;
	paths.push_back("test/1.txt");

	converter.convertToBinary(paths);
	loader.getBlocksFromFile("1.bin");
}