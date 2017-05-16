#include "ParallelepipedGenerator.h"
#include <iostream>
#include <fstream>
#include <math.h>

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

bool ParallelepipedGenerator::isBelongsToEllipsoid(Node node) {
	double diffX = node.x - ellipsoidGenerator.getX();
	double diffY = node.y - ellipsoidGenerator.getY();
	double diffZ = node.z - ellipsoidGenerator.getZ();
	double fractionX = pow(diffX, 2) / ellipsoidGenerator.getA();
	double fractionY = pow(diffY, 2) / ellipsoidGenerator.getB();
	double fractionZ = pow(diffZ, 2) / ellipsoidGenerator.getC();
	double result = fractionX + fractionY + fractionZ;

	return result <= 1.00000001;
}

void ParallelepipedGenerator::createNodes(string fileName) {
	ellipsoidGenerator.loadEllipsoidProperties(fileName);
	arrayOfNodes = new Node**[xStepsNumber + 1];
	for (int i = 0; i <= xStepsNumber; i++) {
		arrayOfNodes[i] = new Node*[yStepsNumber + 1];
		for (int j = 0; j <= yStepsNumber; j++) {
			arrayOfNodes[i][j] = new Node[zStepsNumber + 1];
		}
	}

	int deletedNodesNumber = 0;
	for (int k = 0; k <= zStepsNumber; k++) {
		for (int j = 0; j <= yStepsNumber; j++) {
			for (int i = 0; i <= xStepsNumber; i++) {
				Node node;
				node.x = i * xStep;
				node.y = j * yStep;
				node.z = k * zStep;
				if (!isBelongsToEllipsoid(node)) {
					node.number = (i + 1) + j * (xStepsNumber + 1) + k * (xStepsNumber + 1) * (yStepsNumber + 1) + minNumber - deletedNodesNumber;
				} else {
					node.number = -1;
					deletedNodesNumber++;
				}
				arrayOfNodes[i][j][k] = node;
			}
		}
	}

	printNodesNumbers();
}

void ParallelepipedGenerator::printNodesNumbers() {
	for (int k = 0; k <= zStepsNumber; k++) {
		for (int j = 0; j <= yStepsNumber; j++) {
			for (int i = 0; i <= xStepsNumber; i++) {
				cout << arrayOfNodes[i][j][k].number << ' ';
			}
			cout << '\n';
		}
		cout << '\n';
	}
}

//void ParallelepipedGenerator::createInternalGrid() {
//	Triangle emptyTriangle;
//	internalGrid.push_back(emptyTriangle);
//
//	for (int i = 0; i < zStepsNumber; i++) {
//		for (int j = 0; j < yStepsNumber; j++) {
//			Triangle triangle;
//			triangle
//		}
//	}
//}