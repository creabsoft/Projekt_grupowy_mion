#pragma once

struct Bessel {
	double before;
	double six60MHz;
	double two80Mhz;

	Bessel(double before, double six60MHz, double two80Mhz) {
		this->before = before;
		this->six60MHz = six60MHz;
		this->two80Mhz = two80Mhz;
	}
};