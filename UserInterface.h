#pragma once

#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <string>
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
	std::cout << "(4) Generate simple histograms." << std::endl;
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
				converter.separateAndSaveChannels(pathToFiles, "binary");
				std::cout << "The conversion to binary has been finished. You can find binary files in binary directory." << std::endl;
				break;
			}
			case 2: {
				pathToFiles = FilesHelper::getPathToFilesFromDirectory("binary");
				converter.convertBinaryToMatlab(pathToFiles, "matlab");
				std::cout << "The conversion to Matlab has been finished. You can find files in matlab folder." << std::endl;
				break;
			}
			case 3: {
				pathToFiles = FilesHelper::getPathToFilesFromDirectory("bessel");
				converter.convertBesselToDamping(pathToFiles, "mion.dat");
				std::cout << "The damping calculations has been finished. Results you can find in dampings directory." << std::endl;
				break;
			}
			case 4: {
				std::cout << "From which file do you want histogram?" << std::endl;
				std::string filename;
				std::cin >> filename;
				
				int option;
				std::cout << "Please select type of histogram: " << std::endl;
				std::cout << "(1) Range: 0.0 to 1.0, 10 compartments" << std::endl;
				std::cout << "(2) Range: 0.5 to 1.0, 10 compartments" << std::endl;
				std::cout << "Your choice: " << std::endl;
				std::cin >> option;

				if (option == 1) {
					converter.generateHistogram(filename, 0.0, 1.0, 10);
				}
				else if (option == 2) {
					converter.generateHistogram(filename, 0.5, 1.0, 10);
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