#include <iostream>
#include "Loader.h"

int main()
{
	Loader loader;
	
	std::vector<std::string> filenames = loader.getFilenamesFromDirectory("test");

	for (auto it : filenames) {
		std::cout << it << std::endl;
	}
}