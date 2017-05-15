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
		file >> x >> y >> z;
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

void EllipsoidGenerator::moveByCenter() {
	int thetaIndex = 2 * thetaLevel + 1;
	int phiIndex = (phiLevel + 1) * 4;
	int internalIndex = internalLevel + 1;

	for (int k = 0; k < internalIndex; k++) {
		for (int i = 0; i < thetaIndex; i++) {
			for (int j = 0; j < phiIndex; j++) {
				arrayOfNodes[i][j][k].x += x;
				arrayOfNodes[i][j][k].y += y;
				arrayOfNodes[i][j][k].z += z;
			}
		}
	}

	int size = axisZNodes.size();
	for (int i = 0; i < size; i++) {
		axisZNodes[i].x += x;
		axisZNodes[i].y += y;
		axisZNodes[i].z += z;
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
	moveByCenter();
	numberNodes();
	printNodes();
}

void EllipsoidGenerator::createExternalGridTopConnections(int phiIndex) {
	externalGrid[1].neighbour.push_back(phiIndex);
	externalGrid[1].neighbour.push_back(phiIndex + 2);
	externalGrid[1].neighbour.push_back(2);

	for (int i = 2; i < phiIndex; i++) {
		externalGrid[i].neighbour.push_back(i - 1);
		externalGrid[i].neighbour.push_back(i * 2 + phiIndex);
		externalGrid[i].neighbour.push_back(i + 1);
	}

	externalGrid[phiIndex].neighbour.push_back(phiIndex - 1);
	externalGrid[phiIndex].neighbour.push_back(3 * phiIndex);
	externalGrid[phiIndex].neighbour.push_back(1);
}

void EllipsoidGenerator::createExternalGridTop() {
	int phiIndex = (phiLevel + 1) * 4;
	int topNumber = axisZNodes[0].number;

	Triangle emptyTriangle;
	externalGrid.push_back(emptyTriangle);

	for (int i = 1; i < phiIndex; i++) {
		Triangle triangle;
		triangle.nodesNumbers.push_back(topNumber);
		triangle.nodesNumbers.push_back(i);
		triangle.nodesNumbers.push_back(i + 1);
		externalGrid.push_back(triangle);
	}
	Triangle triangle;
	triangle.nodesNumbers.push_back(topNumber);
	triangle.nodesNumbers.push_back(phiIndex);
	triangle.nodesNumbers.push_back(1);
	externalGrid.push_back(triangle);

	createExternalGridTopConnections(phiIndex);
}

void EllipsoidGenerator::createExternalGridMiddleConnections(int thetaIndex, int phiIndex) {
	int doublePhiIndex = 2 * phiIndex;
	for (int i = 0; i < thetaIndex - 1; i++) {
		externalGrid[i * doublePhiIndex + phiIndex + 1].neighbour.push_back(i * doublePhiIndex + phiIndex + doublePhiIndex);
		externalGrid[i * doublePhiIndex + phiIndex + 1].neighbour.push_back((i + 1) * doublePhiIndex + phiIndex + 2);
		externalGrid[i * doublePhiIndex + phiIndex + 1].neighbour.push_back(i * doublePhiIndex + phiIndex + 2);

		for (int j = 2; j < doublePhiIndex; j++) {
			int number = i * doublePhiIndex + phiIndex + j;
			if (number % 2 == 0) {
				externalGrid[number].neighbour.push_back(number - 1);
				externalGrid[number].neighbour.push_back(number + 1);
				externalGrid[number].neighbour.push_back(number - doublePhiIndex - 1);
			} else {
				externalGrid[number].neighbour.push_back(number - 1);
				externalGrid[number].neighbour.push_back(number + doublePhiIndex + 1);
				externalGrid[number].neighbour.push_back(number + 1);
			}
		}

		externalGrid[i * doublePhiIndex + phiIndex + doublePhiIndex].neighbour.push_back(i * doublePhiIndex + phiIndex + doublePhiIndex - 1);
		externalGrid[i * doublePhiIndex + phiIndex + doublePhiIndex].neighbour.push_back(i * doublePhiIndex + phiIndex + 1);
		externalGrid[i * doublePhiIndex + phiIndex + doublePhiIndex].neighbour.push_back((i - 1) * doublePhiIndex + phiIndex + doublePhiIndex - 1);
	}

	for (int i = phiIndex + 2; i <= phiIndex + doublePhiIndex; i = i + 2) {
		externalGrid[i].neighbour[2] = (i - phiIndex) / 2;
	}
	int tempIndex = phiIndex + (thetaIndex - 1) * doublePhiIndex;
	for (int i = phiIndex + (thetaIndex - 2) * doublePhiIndex + 1; i <= phiIndex + (thetaIndex - 1) * doublePhiIndex; i = i + 2) {
		tempIndex++;
		externalGrid[i].neighbour[1] = (tempIndex);
	}
}

void EllipsoidGenerator::createExternalGridMiddle() {
	int thetaIndex = 2 * thetaLevel + 1;
	int phiIndex = (phiLevel + 1) * 4;

	for (int i = 0; i < thetaIndex - 1; i++) {
		for (int j = 0; j < phiIndex - 1; j++) {
			Triangle triangleOne;
			triangleOne.nodesNumbers.push_back(i * phiIndex + j + 1);
			triangleOne.nodesNumbers.push_back((i + 1) * phiIndex + j + 1);
			triangleOne.nodesNumbers.push_back((i + 1) * phiIndex + j + 2);
			externalGrid.push_back(triangleOne);

			Triangle triangleTwo;
			triangleTwo.nodesNumbers.push_back(i * phiIndex + j + 1);
			triangleTwo.nodesNumbers.push_back((i + 1) * phiIndex + j + 2);
			triangleTwo.nodesNumbers.push_back(i * phiIndex + j + 2);
			externalGrid.push_back(triangleTwo);
		}
		Triangle triangleOne;
		triangleOne.nodesNumbers.push_back(i * phiIndex + phiIndex);
		triangleOne.nodesNumbers.push_back((i + 1) * phiIndex + phiIndex);
		triangleOne.nodesNumbers.push_back((i + 1) * phiIndex + 1);
		externalGrid.push_back(triangleOne);

		Triangle triangleTwo;
		triangleTwo.nodesNumbers.push_back(i * phiIndex + phiIndex);
		triangleTwo.nodesNumbers.push_back((i + 1) * phiIndex + 1);
		triangleTwo.nodesNumbers.push_back(i * phiIndex + 1);
		externalGrid.push_back(triangleTwo);
	}

	createExternalGridMiddleConnections(thetaIndex, phiIndex);
}

void EllipsoidGenerator::createExternalGridBottomConnections(int thetaIndex, int phiIndex) {
	int doublePhiIndex = 2 * phiIndex;
	externalGrid[phiIndex + (thetaIndex - 1) * doublePhiIndex + 1].neighbour.push_back(phiIndex + (thetaIndex - 1) * doublePhiIndex + phiIndex);
	externalGrid[phiIndex + (thetaIndex - 1) * doublePhiIndex + 1].neighbour.push_back(phiIndex + (thetaIndex - 1) * doublePhiIndex + 2);
	externalGrid[phiIndex + (thetaIndex - 1) * doublePhiIndex + 1].neighbour.push_back(phiIndex + (thetaIndex - 1) * doublePhiIndex + 1 - doublePhiIndex);

	for (int i = 2; i < phiIndex; i++) {
		externalGrid[phiIndex + (thetaIndex - 1) * doublePhiIndex + i].neighbour.push_back(phiIndex + (thetaIndex - 1) * doublePhiIndex + i - 1);
		externalGrid[phiIndex + (thetaIndex - 1) * doublePhiIndex + i].neighbour.push_back(phiIndex + (thetaIndex - 1) * doublePhiIndex + i + 1);
		externalGrid[phiIndex + (thetaIndex - 1) * doublePhiIndex + i].neighbour.push_back(phiIndex + (thetaIndex - 1) * doublePhiIndex - doublePhiIndex + (i * 2) - 1);
	}

	externalGrid[phiIndex + (thetaIndex - 1) * doublePhiIndex + phiIndex].neighbour.push_back(phiIndex + (thetaIndex - 1) * doublePhiIndex + phiIndex - 1);
	externalGrid[phiIndex + (thetaIndex - 1) * doublePhiIndex + phiIndex].neighbour.push_back(phiIndex + (thetaIndex - 1) * doublePhiIndex + 1);
	externalGrid[phiIndex + (thetaIndex - 1) * doublePhiIndex + phiIndex].neighbour.push_back(phiIndex + (thetaIndex - 1) * doublePhiIndex - 1);
}

void EllipsoidGenerator::createExternalGridBottom() {
	int thetaIndex = 2 * thetaLevel + 1;
	int phiIndex = (phiLevel + 1) * 4;
	int bottomIndex = 2 * (internalLevel + 1);
	int bottomNumber = axisZNodes[bottomIndex].number;

	for (int i = (thetaIndex - 1) * phiIndex + 1; i < thetaIndex * phiIndex; i++) {
		Triangle triangle;
		triangle.nodesNumbers.push_back(i);
		triangle.nodesNumbers.push_back(bottomNumber);
		triangle.nodesNumbers.push_back(i + 1);
		externalGrid.push_back(triangle);
	}
	Triangle triangle;
	triangle.nodesNumbers.push_back(thetaIndex * phiIndex);
	triangle.nodesNumbers.push_back(bottomNumber);
	triangle.nodesNumbers.push_back((thetaIndex - 1) * phiIndex + 1);
	externalGrid.push_back(triangle);

	createExternalGridBottomConnections(thetaIndex, phiIndex);
}

void EllipsoidGenerator::createExternalGrid() {
	createExternalGridTop();
	createExternalGridMiddle();
	createExternalGridBottom();
}

void EllipsoidGenerator::createInitialTriangulation() {
	int centralNodeNumber = axisZNodes[internalLevel + 1].number;
	int size = externalGrid.size();
	for (int i = 1; i < size; i++) {
		Tetrahedron tetrahedron;
		tetrahedron.nodesNumbers.push_back(centralNodeNumber);
		for (int j = 0; j < 3; j++) {
			tetrahedron.nodesNumbers.push_back(externalGrid[i].nodesNumbers[j]);
		}
		tetrahedrons.push_back(tetrahedron);
	}

	list<Tetrahedron>::iterator iTetrahedron = tetrahedrons.begin();
	for (int i = 1; i < size; i++) {
		iTetrahedron->neighbour.push_back(0);
		for (int j = 0; j < 3; j++) {
			int k = j + 1;
			if (k == 3) {
				k = 0;
			}

			list<Tetrahedron>::iterator neighbourTetrahedron = tetrahedrons.begin();
			int number = externalGrid[i].neighbour[k];
			for (int n = 1; n < number; n++) {
				neighbourTetrahedron++;
			}
			iTetrahedron->neighbour.push_back(&(*neighbourTetrahedron));
		}
		iTetrahedron++;
	}
}

void EllipsoidGenerator::transferInternalNodes() {
	int thetaIndex = 2 * thetaLevel + 1;
	int phiIndex = (phiLevel + 1) * 4;
	int internalIndex = internalLevel + 1;
	int topIndex = 0;
	int bottomIndex = 2 * (internalLevel + 1);

	for (int k = 1; k < internalIndex; k++) {
		for (int i = 0; i < thetaIndex; i++) {
			for (int j = 0; j < phiIndex; j++) {
				internalNodes.push_back(arrayOfNodes[i][j][k]);
			}
		}

		topIndex++;
		internalNodes.push_back(axisZNodes[topIndex]);
		bottomIndex--;
		internalNodes.push_back(axisZNodes[bottomIndex]);
	}
}

void EllipsoidGenerator::transferAllNodes() {
	int thetaIndex = 2 * thetaLevel + 1;
	int phiIndex = (phiLevel + 1) * 4;
	int internalIndex = internalLevel + 1;
	Node node;
	nodes.push_back(node);
	for (int k = 0; k < internalIndex; k++) {
		for (int i = 0; i < thetaIndex; i++) {
			for (int j = 0; j < phiIndex; j++) {
				nodes.push_back(arrayOfNodes[i][j][k]);
			}
		}
	}

	int axisZNumber = thetaIndex * phiIndex * internalIndex;
	int size = axisZNodes.size();
	for (int i = 0; i < size; i++) {
		axisZNumber++;
		nodes.push_back(axisZNodes[i]);
	}
}

int EllipsoidGenerator::getStatusOfAccessoryToTetrahedron(Node node, Tetrahedron tetrahedron) {
	for (int i = 1; i < 4; i++) {
		Node topNode = nodes[tetrahedron.nodesNumbers[0]];
		Node basisNode = nodes[tetrahedron.nodesNumbers[i]];
		double XNumerator = node.x - topNode.x;
		double YNumerator = node.y - topNode.y;
		double ZNumerator = node.z - topNode.z;
		double XDenominator = basisNode.x - topNode.x;
		double YDenominator = basisNode.y - topNode.y;
		double ZDenominator = basisNode.z - topNode.z;

		vector<double> compare;
		if (abs(XNumerator) > 0.00000001 || abs(XDenominator) > 0.00000001) {
			compare.push_back(XNumerator / XDenominator);
		}
		if (abs(YNumerator) > 0.00000001 || abs(YDenominator) > 0.00000001) {
			compare.push_back(YNumerator / YDenominator);
		}
		if (abs(ZNumerator) > 0.00000001 || abs(ZDenominator) > 0.00000001) {
			compare.push_back(ZNumerator / ZDenominator);
		}

		if (compare.size() == 1 && compare[0] > 0) {
			return i;
		} else if (compare.size() > 1) {
			bool isEqual = true;
			int size = compare.size();
			for (int j = 1; j < size; j++) {
				if (abs(compare[j] - compare[j - 1]) > 0.00000001 || compare[j] < 0) {
					isEqual = false;
				}
			}
			if (isEqual) {
				return i;
			}
		}
	}

	return -1;
}

int EllipsoidGenerator::getIncrementedStatus(int status) {
	status++;
	if (status == 4) {
		return 1;
	}

	return status;
}

void EllipsoidGenerator::deleteGarbage() {
	list<Tetrahedron>::iterator currentTetrahedron = tetrahedrons.begin();
	list<Tetrahedron>::iterator end = tetrahedrons.end();

	while (currentTetrahedron != end) {
		if (currentTetrahedron->neighbour.size() == 0 && currentTetrahedron->nodesNumbers.size() == 0) {
			tetrahedrons.erase(currentTetrahedron++);
		} else {
			currentTetrahedron++;
		}
	}
}

void EllipsoidGenerator::fillNeighours(vector<Tetrahedron*> &neighbours, vector<int> &basisRibNodes, 
									   list<Tetrahedron>::iterator currentTetrahedron, int status) {
	neighbours.push_back(&(*currentTetrahedron));
	basisRibNodes.push_back(status);
	int nodeNumber = currentTetrahedron->nodesNumbers[status];

	for (int i = 0; i < neighbours.size(); i++) {
		Tetrahedron* neighbour = neighbours[i]->neighbour[getIncrementedStatus(basisRibNodes[i])];
		if (neighbours[0] != neighbour) {
			neighbours.push_back(neighbour);
			for (int j = 1; j < 4; j++) {
				if (neighbours[i + 1]->nodesNumbers[j] == nodeNumber) {
					basisRibNodes.push_back(j);
				}
			}
		}
	}
}

void EllipsoidGenerator::addNewTetrahedronsWithNodes(vector<Tetrahedron*> &neighbours, vector<int> &basisRibNodes, int number) {
	for (int i = 0; i < neighbours.size(); i++) {
		Tetrahedron tetrahedron;
		tetrahedron.nodesNumbers.push_back(neighbours[i]->nodesNumbers[0]);
		tetrahedron.nodesNumbers.push_back(number);
		int status = getIncrementedStatus(basisRibNodes[i]);
		tetrahedron.nodesNumbers.push_back(neighbours[i]->nodesNumbers[status]);
		status = getIncrementedStatus(status);
		tetrahedron.nodesNumbers.push_back(neighbours[i]->nodesNumbers[status]);
		tetrahedrons.push_back(tetrahedron);
	}
	for (int i = 0; i < neighbours.size(); i++) {
		Tetrahedron tetrahedron;
		tetrahedron.nodesNumbers.push_back(number);
		int status = basisRibNodes[i];
		tetrahedron.nodesNumbers.push_back(neighbours[i]->nodesNumbers[status]);
		for (int j = 0; j < 2; j++) {
			status = getIncrementedStatus(status);
			tetrahedron.nodesNumbers.push_back(neighbours[i]->nodesNumbers[status]);
		}
		tetrahedrons.push_back(tetrahedron);
	}
}

void EllipsoidGenerator::addInternalNodesToTriangulation() {
	int size = internalNodes.size();
	for (int i = 0; i < size; i++) {
		list<Tetrahedron>::iterator currentTetrahedron = tetrahedrons.begin();
		list<Tetrahedron>::iterator end = tetrahedrons.end();
		while (currentTetrahedron != end) {
			int status = getStatusOfAccessoryToTetrahedron(internalNodes[i], *currentTetrahedron);
			if (status != -1) {
				vector<Tetrahedron*> neighbours;
				vector<int> basisRibNodes;
				fillNeighours(neighbours, basisRibNodes, currentTetrahedron, status);
				addNewTetrahedronsWithNodes(neighbours, basisRibNodes, internalNodes[i].number);

				int doubleSize = 2 * neighbours.size();
				list<Tetrahedron>::iterator newTetrahedron = tetrahedrons.end();
				for (int j = 0; j < doubleSize; j++) {
					newTetrahedron--;
				}
				list<Tetrahedron>::iterator upFirst = newTetrahedron;
				list<Tetrahedron>::iterator upLast = upFirst;
				for (int j = 0; j < neighbours.size() - 1; j++) {
					upLast++;
				}
				list<Tetrahedron>::iterator downFirst = upLast;
				downFirst++;
				list<Tetrahedron>::iterator downLast = downFirst;
				for (int j = 0; j < neighbours.size() - 1; j++) {
					downLast++;
				}

				for (int j = 0; j < neighbours.size(); j++) {
					list<Tetrahedron>::iterator partOfInitialTetrahedron = newTetrahedron;
					for (int k = 0; k < neighbours.size(); k++) {
						partOfInitialTetrahedron++;
					}
					newTetrahedron->neighbour.push_back(&(*partOfInitialTetrahedron));
					Tetrahedron* externalTetrahedron = neighbours[j]->neighbour[basisRibNodes[j]];
					for (int k = 0; k < 4; k++) {
						if (externalTetrahedron->neighbour[k] == neighbours[j]) {
							externalTetrahedron->neighbour[k] = &(*newTetrahedron);
						}
					}
					newTetrahedron->neighbour.push_back(externalTetrahedron);
					list<Tetrahedron>::iterator rightNeighbour = newTetrahedron;
					rightNeighbour++;
					newTetrahedron->neighbour.push_back(&(*rightNeighbour));
					list<Tetrahedron>::iterator leftNeighbour = newTetrahedron;
					leftNeighbour--;
					newTetrahedron->neighbour.push_back(&(*leftNeighbour));

					newTetrahedron++;
				}
				upFirst->neighbour[3] = &(*upLast);
				upLast->neighbour[2] = &(*upFirst);

				for (int j = 0; j < neighbours.size(); j++) {
					Tetrahedron* externalTetrahedron = neighbours[j]->neighbour[0];
					if (externalTetrahedron != 0) {
						for (int k = 0; k < 4; k++) {
							if (externalTetrahedron->neighbour[k] == neighbours[j]) {
								externalTetrahedron->neighbour[k] = &(*newTetrahedron);
							}
						}
					}
					newTetrahedron->neighbour.push_back(externalTetrahedron);
					list<Tetrahedron>::iterator partOfInitialTetrahedron = newTetrahedron;
					for (int k = 0; k < neighbours.size(); k++) {
						partOfInitialTetrahedron--;
					}
					newTetrahedron->neighbour.push_back(&(*partOfInitialTetrahedron));
					list<Tetrahedron>::iterator rightNeighbour = newTetrahedron;
					rightNeighbour++;
					if (rightNeighbour != end) {
						newTetrahedron->neighbour.push_back(&(*rightNeighbour));
					} else {
						newTetrahedron->neighbour.push_back(0);
					}
					list<Tetrahedron>::iterator leftNeighbour = newTetrahedron;
					leftNeighbour--;
					newTetrahedron->neighbour.push_back(&(*leftNeighbour));

					newTetrahedron++;
				}
				downFirst->neighbour[3] = &(*downLast);
				downLast->neighbour[2] = &(*downFirst);

				for (int j = 0; j < neighbours.size(); j++) {
					neighbours[j]->nodesNumbers.clear();
					neighbours[j]->neighbour.clear();
				}

				deleteGarbage();
				currentTetrahedron = end;
			} else {
				currentTetrahedron++;
			}
		}
	}
}

void EllipsoidGenerator::createTriangulation() {
	createInitialTriangulation();
	transferInternalNodes();
	transferAllNodes();
	addInternalNodesToTriangulation();
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

void EllipsoidGenerator::writeNodesInFile(string fileName) {
	ofstream file(fileName);

	int size = nodes.size();
	file << size - 1 << " 3\n";
	for (int i = 1; i < size; i++) {
		Node node = nodes[i];
		file << node.x << ' ' << node.y << ' ' << node.z << '\n';
	}

	file << '\n';

	file.close();
}

void EllipsoidGenerator::writeTetrahedronsInFile(string fileName) {
	ofstream file(fileName, ios_base::app);

	file << tetrahedrons.size() << " 4\n";

	list<Tetrahedron>::iterator currentTetrahedron = tetrahedrons.begin();
	list<Tetrahedron>::iterator end = tetrahedrons.end();
	while (currentTetrahedron != end) {
		file << 2;

		for (int i = 0; i < 4; i++) {
			file << ' ' << currentTetrahedron->nodesNumbers[i];
		}

		file << "\n";

		currentTetrahedron++;
	}

	file << '\n';

	file.close();
}

void EllipsoidGenerator::writeGidFile() {
	ofstream file("GidResult.txt");
	list<Tetrahedron>::iterator currentTetrahedron = tetrahedrons.begin();
	list<Tetrahedron>::iterator end = tetrahedrons.end();
	file << "mesh dimension = 3 elemtype tetrahedra nnode = 4\ncoordinates\n";
	for (int i = 1; i < nodes.size(); i++) {
		Node node = nodes[i];
		file << node.number << " " << node.x << ' ' << node.y << ' ' << node.z << '\n';
	}
	file << "end coordinates\nelements\n";
	int number = 0;
	while (currentTetrahedron != end) {
		number++;
		file << number;
		for (int i = 0; i < 4; i++) {
			file << ' ' << currentTetrahedron->nodesNumbers[i];
		}

		file << " 1\n";

		currentTetrahedron++;
	}
	file << "end elements";

	file.close();
}

int EllipsoidGenerator::getMaxNumber() {
	return maxNumber;
}