#pragma once
#include <Vector>
#include "BesselName.h"

class BesselNameHelper {
public:
	static BesselName getBesselNameForSelectedFile(std::vector<BesselName> besselsNames, std::string filename);
};

BesselName BesselNameHelper::getBesselNameForSelectedFile(std::vector<BesselName> besselsNames, std::string filename) {
	for (auto it : besselsNames) {
		if (filename == it.oldName) {
			return it;
		}
	}

	return BesselName(filename, filename);
}