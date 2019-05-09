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
	Histogram(std::vector<Damping> dampings, double rangeFrom, double rangeTo, int numberOfCompartments);
	Histogram(std::vector<int> histogramSix, std::vector<int> histogramTwo, int totalNumber);
	~Histogram();

	void setHistogramSix(std::vector<int> histogramSix);
	void setHistogramTwo(std::vector<int> histogramTwo);
	void setHistogramDampings(std::vector<Damping> dampings);
	void setTotalNumber(int totalNumber);

	void generateSimpleHistograms();

	void generateLabels();

private:
	std::vector<Damping> dampings;
	std::vector<int> histogramSix;
	std::vector<int> histogramTwo;
	std::vector<std::string> ranges = { "<0.0, 0.1)", "<0.1, 0.2)", "<0.2, 0,3)", "<0.3, 0.4)", "<0.4, 0.5)", "<0.5, 0.6)", "<0.6, 0.7)", "<0.7, 0.8)", "<0.8, 0.9)", "<0.9, 1.0)" };
	int totalNumber;
	int numberOfCompartments;
	double range;
	double rangeFrom;
	double rangeTo;
	double d;
};

Histogram::Histogram()
{
}

Histogram::Histogram(std::vector<Damping> dampings, double rangeFrom, double rangeTo, int numberOfCompartments) {
	this->dampings = dampings;
	this->rangeFrom = rangeFrom;
	this->rangeTo = rangeTo;
	this->range = rangeTo - rangeFrom;
	this->numberOfCompartments = numberOfCompartments;
	this->d = range / (double) numberOfCompartments;
}

Histogram::Histogram(std::vector<int> histogramSix, std::vector<int> histogramTwo, int totalNumber) {
	this->histogramSix = histogramSix;
	this->histogramTwo = histogramTwo;
	this->totalNumber = totalNumber;
}


Histogram::~Histogram()
{
}

void Histogram::setHistogramSix(std::vector<int> histogramSix) {
	this->histogramSix = histogramSix;
}

void Histogram::setHistogramTwo(std::vector<int> histogramTwo) {
	this->histogramTwo = histogramTwo;
}

void Histogram::setTotalNumber(int totalNumber) {
	this->totalNumber = totalNumber;
}

void Histogram::generateLabels() {
	ranges.clear();
	int numFrom = rangeFrom;

	for (int i = 0; i < numberOfCompartments; i++) {
		ranges.push_back("<" + std::to_string(rangeFrom).substr(0,4) + ", " + std::to_string(rangeFrom + d).substr(0,4) + ")");
		rangeFrom += d;
	}
}

void Histogram::generateSimpleHistograms() {
	std::vector<int> histogramSix;
	std::vector<int> histogramTwo;
	generateLabels();
	for (int i = 0; i < numberOfCompartments; i++) {
		histogramSix.push_back(0);
		histogramTwo.push_back(0);
	}
	
	int deleteValue = 0;
	if (range == 0.5) {
		deleteValue = 10;
	}

	for (Damping damping : dampings) {
		histogramSix[(int)(damping.six60MHz / d) - deleteValue]++;
		histogramTwo[(int)(damping.two80MHz / d) - deleteValue]++;
	}

	this->histogramSix = histogramSix;
	this->histogramTwo = histogramTwo;
	this->totalNumber = dampings.size();

	std::vector<int> percentageSix = HistogramHelper::calculatePercentage(histogramSix, totalNumber);
	std::vector<int> percentageTwo = HistogramHelper::calculatePercentage(histogramTwo, totalNumber);
	std::ofstream sixFile("histogramSix60MHz.txt");
	std::ofstream twoFile("histogramTwo80MHz.txt");
	if (sixFile.is_open() && twoFile.is_open()) {
		sixFile << "          \t |1---5---10--------20--------30--------40--------50--------60--------70--------80--------90-------100" << std::endl;
		for (int i = 0; i < percentageSix.size(); i++) {
			sixFile << ranges[i] << "\t |";
			for (int j = 0; j < percentageSix[i]; j++) {
				sixFile << "*";
			}
			sixFile << " " << percentageSix[i] << "% (" << histogramSix[i] << ")";
			sixFile << std::endl;
		}
		sixFile << "          \t |1---5---10--------20--------30--------40--------50--------60--------70--------80--------90-------100" << std::endl;

		twoFile << "          \t |1---5---10--------20--------30--------40--------50--------60--------70--------80--------90-------100" << std::endl;
		for (int i = 0; i < percentageTwo.size(); i++) {
			twoFile << ranges[i] << "\t |";
			for (int j = 0; j < percentageTwo[i]; j++) {
				twoFile << "*";
			}
			twoFile << " " << percentageTwo[i] << "% (" << histogramTwo[i] << ")";
			twoFile << std::endl;
		}
		twoFile << "          \t |1---5---10--------20--------30--------40--------50--------60--------70--------80--------90-------100" << std::endl;
	}
}