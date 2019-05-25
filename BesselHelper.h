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
	static double getMaximumFromVector(std::vector<double> values);
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
	std::vector<double> before;
	std::vector<double> six60MHz;
	std::vector<double> two80MHz;
	std::map<int, Bessel> maximumBessels;
	int b, s, t;
	b = s = t = 0;

	for (int i = 0; i < bessels.size() - 1; i++) {
		if ((bessels[i + 1].before - bessels[i].before) > 2) {
			compartmentBefore = true;
		}
		if ((bessels[i].before - bessels[i + 1].before) > 2) {
			compartmentBefore = false;
		}


		if (compartmentBefore) {
			before.push_back(bessels[i].before);
		}
		if (!compartmentBefore) {
			if (before.size() > 0) {
				double maximumBefore = getMaximumFromVector(before);

				maximumBessels[b++].before = maximumBefore;
				before.clear();
			}
		}

		if (compartmentSix60MHz) {
			six60MHz.push_back(bessels[i].six60MHz);
		}
		if (!compartmentSix60MHz) {
			if (six60MHz.size() > 0) {
				double maximumSix60MHz = getMaximumFromVector(six60MHz);

				maximumBessels[s++].six60MHz = maximumSix60MHz;
				six60MHz.clear();
			}
		}

		if (compartmentTwo80MHz) {
			two80MHz.push_back(bessels[i].two80Mhz);
		}
		if (!compartmentTwo80MHz) {
			if (two80MHz.size() > 0) {
				double maximumTwo80MHz = getMaximumFromVector(two80MHz);

				maximumBessels[t++].two80Mhz = maximumTwo80MHz;
				two80MHz.clear();
			}
		}
	}

	for (auto it : maximumBessels) {
		std::cout << it.second.before << " " << it.second.six60MHz << " " << it.second.two80Mhz << std::endl;
	}
}

double BesselHelper::getMaximumFromVector(std::vector<double> values) {
	double maximum = 0.0;

	for (auto it : values) {
		if(it > maximum) {
			maximum = it;
		}
	}

	return maximum;
}