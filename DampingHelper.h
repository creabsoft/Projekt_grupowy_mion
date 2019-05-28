#pragma once

#include <vector>
#include "Damping.h"

class DampingHelper {
public:
	static double getMaximumSix60MHz(std::vector<Damping> dampings);
	static double getMaximumTwo80MHz(std::vector<Damping> dampings);

	static double getMinimumSix60MHz(std::vector<Damping> dampings);
	static double getMinimumTwo80MHz(std::vector<Damping> dampings);
};

double DampingHelper::getMaximumSix60MHz(std::vector<Damping> dampings) {
	double six60MHz = 0.0;
	for (Damping damping : dampings) {
		if (damping.six60MHz > six60MHz) {
			six60MHz = damping.six60MHz;
		}

		return six60MHz;
	}
}

double DampingHelper::getMaximumTwo80MHz(std::vector<Damping> dampings) {
	double two80MHz = 0.0;
	for (Damping damping : dampings) {
		if (damping.two80MHz > two80MHz) {
			two80MHz = damping.two80MHz;
		}

		return two80MHz;
	}
}

double DampingHelper::getMinimumSix60MHz(std::vector<Damping> dampings) {
	double six60MHz = 1.0;
	for (Damping damping : dampings) {
		if (damping.six60MHz < six60MHz) {
			six60MHz = damping.six60MHz;
		}

		return six60MHz;
	}
}

double DampingHelper::getMinimumTwo80MHz(std::vector<Damping> dampings) {
	double two80MHz = 1.0;
	for (Damping damping : dampings) {
		if (damping.two80MHz > two80MHz) {
			two80MHz = damping.two80MHz;
		}

		return two80MHz;
	}
}