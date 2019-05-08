#pragma once

#include <iostream>
#include <stdlib.h>
#include "Converter.h"
#include "FilesHelper.h"
#include "SingleBlock.h"

class UserInterface
{
public:
	UserInterface();
	~UserInterface();

	int getUserChoice();
	void runUserAction();

	Converter converter;
};

UserInterface::UserInterface()
{
}


UserInterface::~UserInterface()
{
}

int UserInterface::getUserChoice() {
	std::cout << "********************************" << std::endl;
	std::cout << "Please choose the program action:" << std::endl;
	std::cout << "--------------------------------" << std::endl;
	std::cout << "(1) Convert from txt to binary."<< std::endl;
	std::cout << "(2) Convert from binary to Matlab." << std::endl;
	std::cout << "(3) Calculating damping for the Bessel files." << std::endl;
	std::cout << "(4) Convert binary files to categories." << std::endl;
	std::cout << "(5) Generate simple histograms." << std::endl;
	std::cout << "(0) Exit the program." << std::endl;
	std::cout << "-------------------------------" << std::endl;
	std::cout << "Your choice: ";

	int userChoice = 0;
	std::cin >> userChoice;

	return userChoice;
}

void UserInterface::runUserAction() {
	bool exit = false;

	while (exit != true) {
		int userChoice = getUserChoice();
		std::vector<std::string> pathToFiles;

		switch (userChoice) {
			case 1: {
				pathToFiles = FilesHelper::getPathToFilesFromDirectory("input");
				converter.separateAndSaveChannels(pathToFiles);
				std::cout << "The conversion to binary has been finished. You can find binary files in binary directory." << std::endl;
				break;
			}
			case 2: {
				pathToFiles = FilesHelper::getPathToFilesFromDirectory("binary");
				converter.convertBinaryToMatlab(pathToFiles);
				std::cout << "The conversion to Matlab has been finished. You can find files in matlab folder." << std::endl;
				break;
			}
			case 3: {
				pathToFiles = FilesHelper::getPathToFilesFromDirectory("bessel");
				converter.convertBesselToDamping(pathToFiles);
				std::cout << "The damping calculations has been finished. Results you can find here: dampings.txt" << std::endl;
				break;
			}
			case 4: {
				pathToFiles = FilesHelper::getPathToFilesFromDirectory("binary");
				converter.convertFilesToCategories(pathToFiles);
				std::cout << "Finished converting files into categories. You can find them in categories directory." << std::endl;
				break;
			}
			case 5: {
				std::cout << "From which file do you want histogram?" << std::endl;
				std::string filename;
				std::cin >> filename;

				if (filename == "ch1_dampings.txt" || filename == "ch2_dampings.txt" || filename == "ch3_dampings.txt") {
					converter.generateHistogram(filename);
				}
				break;
			}
			case 0: {
				exit = true;
				break;
			}
		}
	}
}