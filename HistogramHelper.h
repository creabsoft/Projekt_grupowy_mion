#pragma once

#include <vector>
#include "Damping.h"
#include "DampingHelper.h"

class HistogramHelper {
public:
	static std::vector<int> calculatePercentage(std::vector<int> counted, int total);
};

std::vector<int> HistogramHelper::calculatePercentage(std::vector<int> counted, int total) {
	std::vector<double> calculated;
	std::vector<int> indexesToAdd;
	int totalPercentage = 0;
	for (int i = 0; i < counted.size(); i++) {
		double percent = (counted[i] / (double)total * 100);
		totalPercentage += (int) percent;
		calculated.push_back(percent);
		indexesToAdd.push_back(i);
	}

	for (int i = 0; i < calculated.size(); i++) {
		double decimal = calculated[i] - (int)calculated[i];
		for (int j = 0; j < calculated.size(); j++) {
			double innerDecimal = calculated[j] - (int)calculated[j];
			if (decimal < innerDecimal) {
				std::swap(indexesToAdd[i], indexesToAdd[j]);
			}
		}
	}

	int divide = 100 - totalPercentage;
	for (int i = 0; i < divide; i++) {
		if (calculated[indexesToAdd[i]] == 0.0) {
			calculated[indexesToAdd[0]]++;
		}
		else {
			calculated[indexesToAdd[i]]++;
		}
	}

	counted.clear();
	for (int i = 0; i < calculated.size(); i++) {
		counted.push_back(static_cast<int>(calculated[i]));
	}

	return counted;
}