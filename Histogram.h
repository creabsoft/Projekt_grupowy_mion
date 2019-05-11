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
	Histogram(std::vector<Damping> dampings, double rangeFrom, double rangeTo, int numberOfCompartments);
	~Histogram();

	void generateSimpleHistograms();
	void generateCsv();

private:
	void generateLabels();
	void generateDirectories();
	std::vector<Damping> dampings;
	std::vector<int> histogramSix;
	std::vector<int> histogramTwo;
	std::vector<int> percentageTwo;
	std::vector<int> percentageSix;
	std::vector<std::string> ranges;
	int totalNumber;
	int numberOfCompartments;
	double range;
	double rangeFrom;
	double rangeTo;
	double d;
};

Histogram::Histogram(std::vector<Damping> dampings, double rangeFrom, double rangeTo, int numberOfCompartments) {
	this->dampings = dampings;
	this->rangeFrom = rangeFrom;
	this->rangeTo = rangeTo;
	this->range = rangeTo - rangeFrom;
	this->numberOfCompartments = numberOfCompartments;
	this->d = range / (double)numberOfCompartments;
}

Histogram::~Histogram()
{
}

void Histogram::generateLabels() {
	ranges.clear();
	int numFrom = rangeFrom;

	for (int i = 0; i < numberOfCompartments; i++) {
		ranges.push_back("<" + std::to_string(rangeFrom).substr(0,4) + ", " + std::to_string(rangeFrom + d).substr(0,4) + ")");
		rangeFrom += d;
	}
}

void Histogram::generateDirectories() {
	if (fs::exists("histograms")) {
		fs::remove_all("histograms");
	}
		
	fs::create_directory("histograms");
}

void Histogram::generateSimpleHistograms() {
	generateDirectories();
	generateLabels();
	for (int i = 0; i < numberOfCompartments; i++) {
		this->histogramSix.push_back(0);
		this->histogramTwo.push_back(0);
	}
	
	int deleteValue = 0;
	if (range == 0.5) {
		deleteValue = 10;
	}

	for (Damping damping : dampings) {
		this->histogramSix[(int)(damping.six60MHz / d) - deleteValue]++;
		this->histogramTwo[(int)(damping.two80MHz / d) - deleteValue]++;
	}

	this->totalNumber = dampings.size();

	this->percentageSix = HistogramHelper::calculatePercentage(histogramSix, totalNumber);
	this->percentageTwo = HistogramHelper::calculatePercentage(histogramTwo, totalNumber);
	std::ofstream sixFile("histograms\\histogramSix60MHz.txt");
	std::ofstream twoFile("histograms\\histogramTwo80MHz.txt");
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

		generateCsv();
	}
}

void Histogram::generateCsv() {
	std::ofstream outputCsvSix("histograms\\histogram_six60MHz.csv");
	std::ofstream outputCsvTwo("histograms\\histogram_two80MHz.csv");

	if (outputCsvSix.is_open() && outputCsvTwo.is_open()) {
		for (int i = 0; i < numberOfCompartments; i++) {
			outputCsvSix << ranges[i] << ";" << percentageSix[i] << ";" << histogramSix[i] << std::endl;
			outputCsvTwo << ranges[i] << ";" << percentageTwo[i] << ";" << histogramTwo[i] << std::endl;
		}
	}

	outputCsvSix.close();
	outputCsvTwo.close();
}
