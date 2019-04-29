#pragma once

struct Damping {
	double six60MHz;
	double two80MHz;

	Damping(double six60MHz, double two80MHz) {
		this->six60MHz = six60MHz;
		this->two80MHz = two80MHz;
	};
};