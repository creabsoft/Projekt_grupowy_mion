#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include "HistogramHelper.h"

class Histogram
{
public:
	Histogram();
	Histogram(std::vector<int> histogramSix, std::vector<int> histogramTwo, int totalNumber);
	~Histogram();
	void generateHistograms();
	void generateSimpleHistograms();

private:
	std::vector<int> histogramSix;
	std::vector<int> histogramTwo;
	std::vector<std::string> ranges = { "<0.0, 0.1)", "<0.1, 0.2)", "<0.2, 0,3)", "<0.3, 0.4)", "<0.4, 0.5)", "<0.5, 0.6)", "<0.6, 0.7)", "<0.7, 0.8)", "<0.8, 0.9)", "<0.9, 1.0)" };
	int totalNumber;
};

Histogram::Histogram()
{
}

Histogram::Histogram(std::vector<int> histogramSix, std::vector<int> histogramTwo, int totalNumber) {
	this->histogramSix = histogramSix;
	this->histogramTwo = histogramTwo;
	this->totalNumber = totalNumber;
}


Histogram::~Histogram()
{
}

void Histogram::generateHistograms() {
	//Gnuplot gp();
	std::vector<std::pair<std::string, int>> withLabels;

	for (int i = 0; i < histogramSix.size(); i++) {
		withLabels.push_back(std::make_pair(ranges[i], histogramSix[i]));
	}

	// gp << "plot" << gp.file1d(withLabels) << "with lines title'cubic'" << std::endl;
}

void Histogram::generateSimpleHistograms() {
	std::ofstream sixFile("histogramSix60MHz");
	std::ofstream twoFile("histogramTwo80MHz");
	sixFile << "          \t |  1  2  3  4  5  6  7  8  9 10" << std::endl;;
	if (sixFile.is_open() && twoFile.is_open()) {
		for (int i = 0; i < this->histogramSix.size(); i++) {
			sixFile << ranges[i] << "\t |";
			std::cout << histogramSix[i] / (double) totalNumber << std::endl;
			int howManyTimes = (histogramSix[i] / (double) totalNumber) * 10;
			for (int j = 0; j < howManyTimes; j++) {
				sixFile << "***";
			}
			sixFile << std::endl;
		}
		sixFile << "          \t |  1  2  3  4  5  6  7  8  9 10" << std::endl;;
	}
}