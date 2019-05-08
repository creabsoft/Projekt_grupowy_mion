#pragma once

struct Bessel {
	double before;
	double six60MHz;
	double two80Mhz;

	Bessel() {
		this->before = 0.0;
		this->six60MHz = 0.0;
		this->two80Mhz = 0.0;
	}

	Bessel(double before, double six60MHz, double two80Mhz) {
		this->before = before;
		this->six60MHz = six60MHz;
		this->two80Mhz = two80Mhz;
	}
};