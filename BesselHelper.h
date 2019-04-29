#pragma once
#include <vector>
#include "Bessel.h"
#include "Damping.h"

class BesselHelper {
public:
	static Bessel getMaximumBessel(std::vector<Bessel> bessels);
	static Damping calculateDampingFromBessel(Bessel bessel);
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

