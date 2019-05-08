#pragma once

struct Damping {
	double six60MHz;
	double two80MHz;

	Damping() {
		this->six60MHz = 0.0;
		this->two80MHz = 0.0;
	}

	Damping(double six60MHz, double two80MHz) {
		this->six60MHz = six60MHz;
		this->two80MHz = two80MHz;
	};
};