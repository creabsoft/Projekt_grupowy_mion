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
	std::cout << "Prosze wybrac akcje programu:" << std::endl;
	std::cout << "--------------------------------" << std::endl;
	std::cout << "(1) Konwersja plikow tekstowych na binarne."<< std::endl;
	std::cout << "(2) Konwersja plikow binarnych do Matlaba." << std::endl;
	std::cout << "(0) Wyjscie z programu." << std::endl;
	std::cout << "-------------------------------" << std::endl;
	std::cout << "Twoj wybor: ";

	int userChoice = 0;
	std::cin >> userChoice;

	return userChoice;
}

void UserInterface::runUserAction() {
	bool exit = false;

	while (exit != true) {
		int userChoice = getUserChoice();

		switch (userChoice) {
			case 1: {
				std::vector<std::string> pathToFiles = FilesHelper::getPathToFilesFromDirectory("input");
				converter.separateAndSaveChannels(pathToFiles);
				break;
			}
			case 2: {
				std::vector<std::string> pathToFiles = FilesHelper::getPathToFilesFromDirectory("binary");
				converter.convertBinaryToMatlab(pathToFiles);
				break;
			}
			case 0: {
				exit = true;
				break;
			}
		}
	}
}