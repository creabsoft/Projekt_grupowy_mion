#pragma once
#include <vector>
#include <map>
#include "Bessel.h"
#include "Damping.h"

class BesselHelper {
public:
	static Bessel getMaximumBessel(std::vector<Bessel> bessels);
	static Damping calculateDampingFromBessel(Bessel bessel);
	static void getDampingsFromBessels(std::vector<Bessel> bessels);
	static std::pair<int, double> getMaximumFromMap(std::map<int, double> values);
	static std::pair<int, double> getMaximumFromCompartment(std::map<int, int> indexes, std::map<int, double> & maximums, int compartment);
	static int getNumberOfCompartment(std::map<int, int> indexes, int numOfIndex);
};

Bessel BesselHelper::getMaximumBessel(std::vector<Bessel> bessels) {
	double maximumBefore = 0.0;
	double maximumSix60MHz = 0.0;
	double maximumTwo80MHz = 0.0;

	for (Bessel bessel : bessels) {
		if (bessel.before > maximumBefore) {
			maximumBefore = bessel.before;
		}
		if (bessel.six60MHz > maximumSix60MHz) {
			maximumSix60MHz = bessel.six60MHz;
		}
		if (bessel.two80Mhz > maximumTwo80MHz) {
			maximumTwo80MHz = bessel.two80Mhz;
		}
	}

	return Bessel(maximumBefore, maximumSix60MHz, maximumTwo80MHz);
}

Damping BesselHelper::calculateDampingFromBessel(Bessel bessel) {
	return Damping(bessel.six60MHz / bessel.before, bessel.two80Mhz / bessel.before);
}

void BesselHelper::getDampingsFromBessels(std::vector<Bessel> bessels) {
	bool compartmentBefore = false;
	bool compartmentSix60MHz = false;
	bool compartmentTwo80MHz = false;
	std::map<int, double> tempBefore;
	std::map<int, double> tempSix60MHz;
	std::map<int, double> tempTwo80MHz;

	std::map<int, double> maximumBefore;
	std::map<int, double> maximumSix60MHz;
	std::map<int, double> maximumTwo80MHz;

	std::map<int, int> indexes;
	int numOfCompartment = 0;
	std::vector<Damping> maximumDampings;
	int b, s, t;
	b = s = t = 0;
	double maxBefore = -1;
	double maxSix60MHz = -1;
	double maxTwo80MHz = -1;

	for (int i = 0; i < bessels.size() - 1; i++) {
		if (maxBefore == -1 && (bessels[i + 1].before - bessels[i].before) > 200) {
			compartmentBefore = true;
			maxBefore = bessels[i].before;
		}
		if (maxBefore != -1 && bessels[i].before < (5 * maxBefore / 100)) {
			compartmentBefore = false;
			maxBefore = -1;
		}

		if (maxSix60MHz == -1 && (bessels[i + 1].six60MHz - bessels[i].six60MHz) > 200) {
			compartmentSix60MHz = true;
			maxSix60MHz = bessels[i].six60MHz;
		}
		if (maxSix60MHz != -1 && bessels[i].six60MHz < (5 * maxSix60MHz / 100)) {
			compartmentSix60MHz = false;
			maxSix60MHz = -1;
		}

		if (maxTwo80MHz == -1 && (bessels[i + 1].two80Mhz - bessels[i].two80Mhz) > 200) {
			compartmentTwo80MHz = true;
			maxTwo80MHz = bessels[i].two80Mhz;
		}
		if (maxTwo80MHz != -1 && bessels[i].two80Mhz < (5 * maxTwo80MHz / 100)) {
			compartmentTwo80MHz = false;
			maxTwo80MHz = -1;
		}

		if (compartmentBefore) {
			tempBefore[i] = bessels[i].before;
			indexes[i] = numOfCompartment;

			if (maxBefore < bessels[i].before) {
				maxBefore = bessels[i].before;
			}
		}
		if (!compartmentBefore) {
			if (tempBefore.size() > 0) {
				std::pair<int, double> tempMaxBefore = getMaximumFromMap(tempBefore);
				maximumBefore[tempMaxBefore.first] = tempMaxBefore.second;
				numOfCompartment++;
//				maximumBessels[b++].before = maximumBefore.second;
				tempBefore.clear();
			}
		}

		if (compartmentSix60MHz) {
			tempSix60MHz[i] = bessels[i].six60MHz;

			if (maxSix60MHz < bessels[i].six60MHz) {
				maxSix60MHz = bessels[i].six60MHz;
			}
		}
		if (!compartmentSix60MHz) {
			if (tempSix60MHz.size() > 0) {
				std::pair<int, double> tempMaxSix60MHz = getMaximumFromMap(tempSix60MHz);
				maximumSix60MHz[tempMaxSix60MHz.first] = tempMaxSix60MHz.second;
				// maximumBessels[s++].six60MHz = maximumSix60MHz;
				tempSix60MHz.clear();
			}
		}

		if (compartmentTwo80MHz) {
			tempTwo80MHz[i] = bessels[i].two80Mhz;

			if (maxTwo80MHz < bessels[i].two80Mhz) {
				maxTwo80MHz = bessels[i].two80Mhz;
			}
		}
		if (!compartmentTwo80MHz) {
			if (tempTwo80MHz.size() > 0) {
				std::pair<int, double> tempMaxTwo80MHz = getMaximumFromMap(tempTwo80MHz);
				maximumTwo80MHz[tempMaxTwo80MHz.first] = tempMaxTwo80MHz.second;
//				maximumBessels[t++].two80Mhz = maximumTwo80MHz;
				tempTwo80MHz.clear();
			}
		}
	}

	std::cout << maximumBefore.size() << std::endl;
	std::cout << maximumSix60MHz.size() << std::endl;
	std::cout << maximumTwo80MHz.size() << std::endl;
	
	//for (auto it : maximumTwo80MHz) {
	//	std::cout << "MAXIMUM:" << it.second << std::endl;
	//}

	for (auto it : maximumBefore) {
		double beforeMax = it.second;
		int numOfCompartment = getNumberOfCompartment(indexes, it.first);
		std::pair<int, double> maxSix60MHz = getMaximumFromCompartment(indexes, maximumSix60MHz, numOfCompartment);
		std::pair<int, double> maxTwo80MHz = getMaximumFromCompartment(indexes, maximumTwo80MHz, numOfCompartment);

		if (maxSix60MHz.first != -1 && maxTwo80MHz.first != -1) {
			Damping damping;
			damping.six60MHz = maxSix60MHz.second / (double) beforeMax;
			damping.two80MHz = maxTwo80MHz.second / (double) beforeMax;
			std::cout << "BEFORE: " << it.first << " " << it.second << std::endl;
			std::cout << "SIX60MHZ: " << maxSix60MHz.first << " " << maxSix60MHz.second << std::endl;
			std::cout << "TWO80MHZ: " << maxTwo80MHz.first << " " << maxTwo80MHz.second << std::endl;
			maximumDampings.push_back(damping);
		}
	}
	std::cout << maximumDampings.size() << std::endl;
	for (auto it : maximumDampings) {
		std::cout << it.six60MHz << " " << it.two80MHz << std::endl;
	}
}

std::pair<int, double> BesselHelper::getMaximumFromCompartment(std::map<int, int> indexes, std::map<int, double> & maximums, int compartment) {
	for (auto max : maximums) {
		for (auto index : indexes) {
			if (abs(index.first - max.first) < 20 && index.second == compartment) {
				std::pair<int, double> maximum = max;
				maximums.erase(max.first);
				return max;
			}
		}
	}

	return std::make_pair(-1, -1);
}

std::pair<int, double> BesselHelper::getMaximumFromMap(std::map<int, double> values) {
	double maximum = 0.0;
	int index = 0;

	for (auto it : values) {
		if(it.second > maximum) {
			index = it.first;
			maximum = it.second;
		}
	}

	return std::make_pair(index, maximum);
}

int BesselHelper::getNumberOfCompartment(std::map<int, int> indexes, int numOfIndex) {
	for (auto it : indexes) {
		if (it.first == numOfIndex) {
			return it.second;
		}
	}

	return -1;
}