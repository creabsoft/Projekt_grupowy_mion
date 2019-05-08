#pragma once

#include <vector>
#include "Damping.h"
#include "DampingHelper.h"
#include "Histogram.h"

class HistogramHelper {
public:
	static void generateHistogramsWithDynamicRange(int numOfCategory, std::vector<Damping> dampings);
	static void generateHistogram(std::vector<Damping> dampings);
};

void HistogramHelper::generateHistogram(std::vector<Damping> dampings) {
	double d = 0.1;

	std::vector<int> histogramSix;
	std::vector<int> histogramTwo;
	histogramSix.reserve(10);
	histogramTwo.reserve(10);
	for (int i = 0; i < 10; i++) {
		histogramSix.push_back(0);
		histogramTwo.push_back(0);
	}

	for (Damping damping : dampings) {
		histogramSix[(int)(damping.six60MHz / d)]++;
		histogramTwo[(int)(damping.two80MHz / d)]++;
	}

	Histogram histogram(histogramSix, histogramTwo, dampings.size());
	histogram.generateSimpleHistograms();
}

void HistogramHelper::generateHistogramsWithDynamicRange(int numOfRange, std::vector<Damping> dampings) {
	double sixRange = 0.0;
	double twoRange = 0.0;
	double maximumSix = DampingHelper::getMaximumSix60MHz(dampings);
	double maximumTwo = DampingHelper::getMaximumTwo80MHz(dampings);
	double minimumSix = DampingHelper::getMinimumSix60MHz(dampings);
	double minimumTwo = DampingHelper::getMinimumTwo80MHz(dampings);
	sixRange = maximumSix - minimumSix;
	twoRange = maximumTwo - minimumTwo;
	double dSix = sixRange / numOfRange;
	double dTwo = twoRange / numOfRange;
	std::vector<std::string> labelsSix;
	std::vector<std::string> labelsTwo;

	std::vector<int> histogramSix;
	std::vector<int> histogramTwo;
	histogramSix.reserve(numOfRange);
	histogramTwo.reserve(numOfRange);
	for (int i = 0; i < numOfRange; i++) {
		//abelsSix.push_back(minimumSix);
		histogramSix[i] = 0;
		histogramTwo[i] = 0;
	 }
	
	for (Damping damping : dampings) {
		histogramSix[(int)(damping.six60MHz / dSix)]++;
		histogramTwo[(int)(damping.two80MHz / dTwo)]++;
	}
}

