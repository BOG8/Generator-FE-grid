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
		file >> a >> b >> c;
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
			double thetaSin = sin(thetaValues[i]);
			arrayOfNodes[i][j][0].x = a * thetaSin * cos(phiValues[j]);
			arrayOfNodes[i][j][0].y = b * thetaSin * sin(phiValues[j]);
			arrayOfNodes[i][j][0].z = c * cos(thetaValues[i]);
		}
	}

	for (int j = 0; j < phiIndex; j++) {
		arrayOfNodes[thetaIndex - 1][j][0].z = 0;
	}

	for (int i = 0; i < thetaIndex; i++) {
		arrayOfNodes[i][phiIndex - 1][0].x = 0;
	}
}

void EllipsoidGenerator::createMainInternalNodes() {
	int parts = internalLevel + 1;
	int thetaIndex = thetaLevel + 1;
	int phiIndex = phiLevel + 2;
	for (int i = 0; i < thetaIndex; i++) {
		for (int j = 0; j < phiIndex; j++) {
			Node externalNode = arrayOfNodes[i][j][0];
			double x = 0;
			double y = 0;
			double z = 0;
			double xStep = externalNode.x / parts;
			double yStep = externalNode.y / parts;
			double zStep = externalNode.z / parts;

			for (int k = internalLevel; k > 0; k--) {
				x = x + xStep;
				y = y + yStep;
				z = z + zStep;
				arrayOfNodes[i][j][k].x = x;
				arrayOfNodes[i][j][k].y = y;
				arrayOfNodes[i][j][k].z = z;
			}
		}
	}
}

void EllipsoidGenerator::reflectByAxisX() {
	int thetaIndex = thetaLevel + 1;
	int phiIndex = phiLevel + 1;
	int internalIndex = internalLevel + 1;

	for (int i = 0; i < thetaIndex; i++) {
		int reflectPhiIndex = phiIndex * 2;
		for (int j = 0; j < phiIndex; j++) {
			for (int k = 0; k < internalIndex; k++) {
				Node node = arrayOfNodes[i][j][k];
				node.x = -node.x;
				arrayOfNodes[i][reflectPhiIndex][k] = node;
			}
			reflectPhiIndex--;
		}
	}
}

void EllipsoidGenerator::reflectByAxisY() {
	int thetaIndex = thetaLevel + 1;
	int phiIndex = 2 * (phiLevel + 1);
	int internalIndex = internalLevel + 1;

	for (int i = 0; i < thetaIndex; i++) {
		int reflectPhiIndex = 2 * phiIndex - 1;
		for (int j = 1; j < phiIndex; j++) {
			for (int k = 0; k < internalIndex; k++) {
				Node node = arrayOfNodes[i][j][k];
				node.y = -node.y;
				arrayOfNodes[i][reflectPhiIndex][k] = node;
			}
			reflectPhiIndex--;
		}
	}
}

void EllipsoidGenerator::reflectByAxisZ() {
	int phiIndex = 4 * (phiLevel + 1);
	int internalIndex = internalLevel + 1;

	for (int j = 0; j < phiIndex; j++) {
		int reflectThetaIndex = 2 * thetaLevel;
		for (int i = 0; i < thetaLevel; i++) {
			for (int k = 0; k < internalIndex; k++) {
				Node node = arrayOfNodes[i][j][k];
				node.z = -node.z;
				arrayOfNodes[reflectThetaIndex][j][k] = node;
			}
			reflectThetaIndex--;
		}
	}
}

void EllipsoidGenerator::createAxisZNodes() {
	Node node;
	node.x = 0;
	node.y = 0;
	node.z = c;
	axisZNodes.push_back(node);

	int part = internalLevel + 1;
	double zStep = c / part;
	for (int i = 0; i < internalLevel; i++) {
		node.z = node.z - zStep;
		axisZNodes.push_back(node);
	}
	node.z = 0;
	axisZNodes.push_back(node);

	for (int i = internalLevel; i >= 0; i--) {
		node.z = -axisZNodes[i].z;
		axisZNodes.push_back(node);
	}
}

void EllipsoidGenerator::numberNodes() {
	int thetaIndex = 2 * thetaLevel + 1;
	int phiIndex = (phiLevel + 1) * 4;
	int internalIndex = internalLevel + 1;

	for (int k = 0; k < internalIndex; k++) {
		for (int i = 0; i < thetaIndex; i++) {
			for (int j = 0; j < phiIndex; j++) {
				arrayOfNodes[i][j][k].number  = (k * thetaIndex * phiIndex) + (i * phiIndex) + j + 1;
			}
		}
	}

	int axisZNumber = thetaIndex * phiIndex * internalIndex;
	int size = axisZNodes.size();
	for (int i = 0; i < size; i++) {
		axisZNumber++;
		axisZNodes[i].number = axisZNumber;
	}
	maxNumber = axisZNumber;
}

void EllipsoidGenerator::createAllNodes() {
	allocateMemory();
	fillVectors();
	createMainExternalNodes();
	createMainInternalNodes();
	reflectByAxisX();
	reflectByAxisY();
	reflectByAxisZ();
	createAxisZNodes();
	numberNodes();
	printNodes();
}

void EllipsoidGenerator::createExternalGridTopConnections(int phiIndex) {
	externalGrid[1].neighbour.push_back(phiIndex);
	externalGrid[1].neighbour.push_back(1 + phiIndex);
	externalGrid[1].neighbour.push_back(2);

	for (int i = 2; i <= phiIndex; i++) {
		externalGrid[i].neighbour.push_back(i - 1);
		externalGrid[i].neighbour.push_back(i + phiIndex);
		externalGrid[i].neighbour.push_back(i + 1);
	}

	externalGrid[phiIndex].neighbour.push_back(phiIndex - 1);
	externalGrid[phiIndex].neighbour.push_back(2 * phiIndex);
	externalGrid[phiIndex].neighbour.push_back(1);
}

void EllipsoidGenerator::createExternalGridTop() {
	int phiIndex = (phiLevel + 1) * 4;
	int topNumber = axisZNodes[0].number;

	Triangle emptyTriangle;
	emptyTriangle.number = -1;
	externalGrid.push_back(emptyTriangle);

	for (int i = 1; i < phiIndex; i++) {
		Triangle triangle;
		triangle.number = i;
		triangle.nodesNumbers.push_back(topNumber);
		triangle.nodesNumbers.push_back(i);
		triangle.nodesNumbers.push_back(i + 1);
		externalGrid.push_back(triangle);
	}
	Triangle triangle;
	triangle.number = phiIndex;
	triangle.nodesNumbers.push_back(topNumber);
	triangle.nodesNumbers.push_back(phiIndex);
	triangle.nodesNumbers.push_back(1);
	externalGrid.push_back(triangle);

	createExternalGridTopConnections(phiIndex);
}

void EllipsoidGenerator::createExternalGrid() {
	createExternalGridTop();
}

void EllipsoidGenerator::printNodes() {
	int thetaIndex = 2 * thetaLevel + 1;
	int phiIndex = (phiLevel + 1) * 4;
	int internalIndex = internalLevel + 1;

	for (int k = 0; k < internalIndex; k++) {
		for (int i = 0; i < thetaIndex; i++) {
			for (int j = 0; j < phiIndex; j++) {
				Node node = arrayOfNodes[i][j][k];
				cout << node.number << "   " << node.x << ' ' << node.y << ' ' << node.z << '\n';
			}
			cout << '\n';
		}
		cout << "\n\n\n";
	}

	int size = axisZNodes.size();
	for (int i = 0; i < size; i++) {
		Node node = axisZNodes[i];
		cout << node.number << "   " << node.x << ' ' << node.y << ' ' << node.z << '\n';
	}
}