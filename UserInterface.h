#pragma once

#include <iostream>

class UserInterface
{
public:
	UserInterface();
	~UserInterface();

	void runUserSwitch();
	int getUserChoice();

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
	std::cout << "-------------------------------" << std::endl;
	std::cout << "Twoj wybor: ";

	int userChoice = 0;
	std::cin >> userChoice;
}
