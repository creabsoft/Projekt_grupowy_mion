#pragma once
#include <Vector>

struct BesselName {
	std::string newName;
	std::string oldName;

	BesselName() {}

	BesselName(std::string newName, std::string oldName) {
		this->newName = newName;
		this->oldName = oldName;
	}
};