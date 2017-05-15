#include "ParallelepipedGenerator.h"
#include <iostream>
#include <fstream>

ParallelepipedGenerator::ParallelepipedGenerator() {

}

ParallelepipedGenerator::~ParallelepipedGenerator() {

}

void ParallelepipedGenerator::setMinNumber(int number) {
	minNumber = number;
}

void ParallelepipedGenerator::loadEllipsoidProperties(string fileName) {
	ifstream file(fileName);

	if (!file) {
		cout << "File couldn't be open";
	}
	else {
		file >> x >> y >> z;
		file >> xStep >> yStep >> zStep;
	}

	file.close();
}

void ParallelepipedGenerator::correctStep(double &x, double &xStep, int &xStepsNumber) {
	double quotient = x / xStep;
	xStepsNumber = rint(quotient);
	xStep = x / xStepsNumber;
}

void ParallelepipedGenerator::correctAllSteps() {
	correctStep(x, xStep, xStepsNumber);
	correctStep(y, yStep, yStepsNumber);
	correctStep(z, zStep, zStepsNumber);
}