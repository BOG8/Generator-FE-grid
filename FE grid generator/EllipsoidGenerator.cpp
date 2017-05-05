#include "EllipsoidGenerator.h"
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

EllipsoidGenerator::EllipsoidGenerator() {

}

EllipsoidGenerator::~EllipsoidGenerator() {

}


void EllipsoidGenerator::loadEllipsoidProperties(string fileName) {
	ifstream file(fileName);

	if (!file) {
		cout << "EllipsoidGenerator: File couldn't be open";
	} else {
		file >> a >> b >> c >> r;
		file >> thetaLevel >> phiLevel >> internalLevel;
	}

	file.close();
}

void EllipsoidGenerator::allocateMemory() {
	int thetaIndex = 2 * thetaLevel + 1;
	int phiIndex = (phiLevel + 1) * 4;

	arrayOfNodes = new Node**[thetaIndex];
	for (int i = 0; i < thetaIndex; i++) {
		arrayOfNodes[i] = new Node*[phiIndex];
		for (int j = 0; j < phiIndex; j++) {
			arrayOfNodes[i][j] = new Node[internalLevel + 1];
		}
	}
}

void EllipsoidGenerator::fillVectors() {
	double halfOfPI = PI / 2;
	double thetaStep = halfOfPI / (thetaLevel + 1);
	double phiStep = halfOfPI / (phiLevel + 1);
	double value = 0;

	for (int i = 0; i < thetaLevel; i++) {
		value = value + thetaStep;
		thetaValues.push_back(value);
	}
	thetaValues.push_back(halfOfPI);

	value = 0;
	phiValues.push_back(value);
	for (int i = 0; i < phiLevel; i++) {
		value = value + phiStep;
		phiValues.push_back(value);
	}
	phiValues.push_back(halfOfPI);
}

void EllipsoidGenerator::createMainExternalNodes() {
	int thetaIndex = thetaLevel + 1;
	int phiIndex = phiLevel + 2;
	for (int i = 0; i < thetaIndex; i++) {
		for (int j = 0; j < phiIndex; j++) {
			arrayOfNodes[i][j][0].x = sin(thetaValues[i]) * cos(phiValues[j]);
			arrayOfNodes[i][j][0].y = sin(thetaValues[i]) * sin(phiValues[j]);
			arrayOfNodes[i][j][0].z = cos(thetaValues[i]);
		}
	}
}

void EllipsoidGenerator::createExternalNodes() {
	allocateMemory();
	fillVectors();
	createMainExternalNodes();

	//TODO: Create other nodes.

	printNodes();
}

void EllipsoidGenerator::printNodes() {
	int thetaIndex = 2 * thetaLevel + 1;
	int phiIndex = (phiLevel + 1) * 4;
	int internalIndex = internalLevel + 1;

	for (int k = 0; k < internalIndex; k++) {
		for (int i = 0; i < thetaIndex; i++) {
			for (int j = 0; j < phiIndex; j++) {
				Node node = arrayOfNodes[i][j][k];
				cout << node.x << ' ' << node.y << ' ' << node.z << '\n';
			}
			cout << '\n';
		}
		cout << "\n\n\n";
	}
}