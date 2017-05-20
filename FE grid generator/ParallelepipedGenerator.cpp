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

void ParallelepipedGenerator::transferAllNodes() {
	Node emptyNode;
	emptyNode.number = minNumber;
	nodes.push_back(emptyNode);

	for (int k = 0; k <= zStepsNumber; k++) {
		for (int j = 0; j <= yStepsNumber; j++) {
			for (int i = 0; i <= xStepsNumber; i++) {
				Node node;
				node = arrayOfNodes[i][j][k];
				if (node.number != -1) {
					nodes.push_back(node);
				}
			}
		}
	}
}

void ParallelepipedGenerator::createTriangles0ZY() {
	for (int i = 0; i < zStepsNumber; i++) {
		for (int j = 0; j < yStepsNumber; j++) {
			Triangle triangleOne;
			triangleOne.nodesNumbers.push_back(arrayOfNodes[0][j][i].number);
			triangleOne.nodesNumbers.push_back(arrayOfNodes[0][j + 1][i + 1].number);
			triangleOne.nodesNumbers.push_back(arrayOfNodes[0][j][i + 1].number);
			internalGrid.push_back(triangleOne);

			Triangle triangleTwo;
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[0][j][i].number);
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[0][j + 1][i].number);
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[0][j + 1][i + 1].number);
			internalGrid.push_back(triangleTwo);
		}
	}
}

void ParallelepipedGenerator::createTrianglesMXY() {
	for (int i = 0; i < xStepsNumber; i++) {
		for (int j = 0; j < yStepsNumber; j++) {
			Triangle triangleOne;
			triangleOne.nodesNumbers.push_back(arrayOfNodes[i][j][zStepsNumber].number);
			triangleOne.nodesNumbers.push_back(arrayOfNodes[i + 1][j + 1][zStepsNumber].number);
			triangleOne.nodesNumbers.push_back(arrayOfNodes[i + 1][j][zStepsNumber].number);
			internalGrid.push_back(triangleOne);

			Triangle triangleTwo;
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[i][j][zStepsNumber].number);
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[i][j + 1][zStepsNumber].number);
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[i + 1][j + 1][zStepsNumber].number);
			internalGrid.push_back(triangleTwo);
		}
	}
}

void ParallelepipedGenerator::createTrianglesMZY() {
	for (int i = zStepsNumber; i > 0; i--) {
		for (int j = 0; j < yStepsNumber; j++) {
			Triangle triangleOne;
			triangleOne.nodesNumbers.push_back(arrayOfNodes[xStepsNumber][j][i].number);
			triangleOne.nodesNumbers.push_back(arrayOfNodes[xStepsNumber][j + 1][i - 1].number);
			triangleOne.nodesNumbers.push_back(arrayOfNodes[xStepsNumber][j][i - 1].number);
			internalGrid.push_back(triangleOne);

			Triangle triangleTwo;
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[xStepsNumber][j][i].number);
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[xStepsNumber][j + 1][i].number);
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[xStepsNumber][j + 1][i - 1].number);
			internalGrid.push_back(triangleTwo);
		}
	}
}

void ParallelepipedGenerator::createTriangles0XY() {
	for (int i = xStepsNumber; i > 0; i--) {
		for (int j = 0; j < yStepsNumber; j++) {
			Triangle triangleOne;
			triangleOne.nodesNumbers.push_back(arrayOfNodes[i][j][0].number);
			triangleOne.nodesNumbers.push_back(arrayOfNodes[i - 1][j + 1][0].number);
			triangleOne.nodesNumbers.push_back(arrayOfNodes[i - 1][j][0].number);
			internalGrid.push_back(triangleOne);

			Triangle triangleTwo;
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[i][j][0].number);
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[i][j + 1][0].number);
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[i - 1][j + 1][0].number);
			internalGrid.push_back(triangleTwo);
		}
	}
}

void ParallelepipedGenerator::createTriangles0XZ() {
	for (int i = 0; i < xStepsNumber; i++) {
		for (int j = 0; j < zStepsNumber; j++) {
			Triangle triangleOne;
			triangleOne.nodesNumbers.push_back(arrayOfNodes[i][0][j].number);
			triangleOne.nodesNumbers.push_back(arrayOfNodes[i][0][j + 1].number);
			triangleOne.nodesNumbers.push_back(arrayOfNodes[i + 1][0][j + 1].number);
			internalGrid.push_back(triangleOne);

			Triangle triangleTwo;
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[i][0][j].number);
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[i + 1][0][j + 1].number);
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[i + 1][0][j].number);
			internalGrid.push_back(triangleTwo);
		}
	}
}

void ParallelepipedGenerator::createTrianglesMXZ() {
	for (int i = 0; i < xStepsNumber; i++) {
		for (int j = 0; j < zStepsNumber; j++) {
			Triangle triangleOne;
			triangleOne.nodesNumbers.push_back(arrayOfNodes[i][yStepsNumber][j].number);
			triangleOne.nodesNumbers.push_back(arrayOfNodes[i + 1][yStepsNumber][j + 1].number);
			triangleOne.nodesNumbers.push_back(arrayOfNodes[i][yStepsNumber][j + 1].number);
			internalGrid.push_back(triangleOne);

			Triangle triangleTwo;
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[i][yStepsNumber][j].number);
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[i + 1][yStepsNumber][j].number);
			triangleTwo.nodesNumbers.push_back(arrayOfNodes[i + 1][yStepsNumber][j + 1].number);
			internalGrid.push_back(triangleTwo);
		}
	}
}

void ParallelepipedGenerator::createTriangles() {
	Triangle emptyTriangle;
	internalGrid.push_back(emptyTriangle);

	createTriangles0ZY();
	createTrianglesMXY();
	createTrianglesMZY();
	createTriangles0XY();
	createTriangles0XZ();
	createTrianglesMXZ();
}

void ParallelepipedGenerator::createInternalXZConnections(int index) {
	int amountXZTringles = 2 * xStepsNumber * zStepsNumber;
	int doubledZStepNumber = 2 * zStepsNumber;
	int index0 = index + amountXZTringles;
	for (int i = index + 1; i < index0; i = i + 2) {
		internalGrid[i].neighbours.push_back(i - doubledZStepNumber + 1);
		internalGrid[i].neighbours.push_back(i + 3);
		internalGrid[i].neighbours.push_back(i + 1);

		internalGrid[i + 1].neighbours.push_back(i);
		internalGrid[i + 1].neighbours.push_back(i + doubledZStepNumber);
		internalGrid[i + 1].neighbours.push_back(i - 2);
	}

	int indexM = index0 + amountXZTringles;
	for (int i = index0 + 1; i < indexM; i = i + 2) {
		internalGrid[i].neighbours.push_back(i + 1);
		internalGrid[i].neighbours.push_back(i + 3);
		internalGrid[i].neighbours.push_back(i - doubledZStepNumber + 1);

		internalGrid[i + 1].neighbours.push_back(i - 2);
		internalGrid[i + 1].neighbours.push_back(i + doubledZStepNumber);
		internalGrid[i + 1].neighbours.push_back(i);
	}
}

void ParallelepipedGenerator::createInternalConnections() {
	int index = 4 * (yStepsNumber * zStepsNumber + yStepsNumber * xStepsNumber);
	int doubledYStepNumber = 2 * yStepsNumber;
	for (int i = 1; i < index; i = i + 2) {
		internalGrid[i].neighbours.push_back(i + 1);
		internalGrid[i].neighbours.push_back(i + doubledYStepNumber + 1);
		internalGrid[i].neighbours.push_back(i - 1);

		internalGrid[i + 1].neighbours.push_back(i - doubledYStepNumber);
		internalGrid[i + 1].neighbours.push_back(i + 2);
		internalGrid[i + 1].neighbours.push_back(i);
	}

	createInternalXZConnections(index);
}

void ParallelepipedGenerator::createExternalConnections0YZ() {
	int doubledYStepsNumber = 2 * yStepsNumber;
	int mainMaxNumber = 4 * (yStepsNumber * zStepsNumber + yStepsNumber * xStepsNumber);

	int main0 = 1;
	int side0 = mainMaxNumber + 1;
	for (int i = 0; i < zStepsNumber; i++) {
		internalGrid[main0].neighbours[2] = side0;
		internalGrid[side0].neighbours[0] = main0;
		main0 = main0 + doubledYStepsNumber;
		side0 = side0 + 2;
	}

	int mainM = doubledYStepsNumber;
	int sideM = mainMaxNumber + 2 * xStepsNumber * zStepsNumber + 1;
	for (int i = 0; i < zStepsNumber; i++) {
		internalGrid[mainM].neighbours[1] = sideM;
		internalGrid[sideM].neighbours[2] = mainM;
		mainM = mainM + doubledYStepsNumber;
		sideM = sideM + 2;
	}
}

void ParallelepipedGenerator::createExternalConnectionsMXZ() {
	int doubledYStepsNumber = 2 * yStepsNumber;
	int mainMaxNumber = 4 * (yStepsNumber * zStepsNumber + yStepsNumber * xStepsNumber);

	int main0 = 2 * yStepsNumber * zStepsNumber + 1;
	int side0 = mainMaxNumber + 2 * zStepsNumber - 1;
	for (int i = 0; i < xStepsNumber; i++) {
		internalGrid[main0].neighbours[2] = side0;
		internalGrid[side0].neighbours[1] = main0;
		main0 = main0 + doubledYStepsNumber;
		side0 = side0 + 2 * zStepsNumber;
	}

	int mainM = 2 * yStepsNumber * zStepsNumber + doubledYStepsNumber;
	int sideM = mainMaxNumber + 2 * xStepsNumber * zStepsNumber + 2 * zStepsNumber - 1;
	for (int i = 0; i < xStepsNumber; i++) {
		internalGrid[mainM].neighbours[1] = sideM;
		internalGrid[sideM].neighbours[1] = mainM;
		mainM = mainM + doubledYStepsNumber;
		sideM = sideM + 2 * zStepsNumber;
	}
}

void ParallelepipedGenerator::createExternalConnectionsMYZ() {
	int doubledYStepsNumber = 2 * yStepsNumber;
	int mainMaxNumber = 4 * (yStepsNumber * zStepsNumber + yStepsNumber * xStepsNumber);

	int main0 = 2 * (yStepsNumber * zStepsNumber + yStepsNumber * xStepsNumber) + 1;
	int side0 = mainMaxNumber + 2 * zStepsNumber * xStepsNumber;
	for (int i = 0; i < zStepsNumber; i++) {
		internalGrid[main0].neighbours[2] = side0;
		internalGrid[side0].neighbours[1] = main0;
		main0 = main0 + doubledYStepsNumber;
		side0 = side0 - 2;
	}

	int mainM = 2 * (yStepsNumber * zStepsNumber + yStepsNumber * xStepsNumber) + doubledYStepsNumber;
	int sideM = mainMaxNumber + 4 * zStepsNumber * xStepsNumber;
	for (int i = 0; i < zStepsNumber; i++) {
		internalGrid[mainM].neighbours[1] = sideM;
		internalGrid[sideM].neighbours[1] = mainM;
		mainM = mainM + doubledYStepsNumber;
		sideM = sideM - 2;
	}
}

void ParallelepipedGenerator::createExternalConnections0XZ() {
	int doubledYStepsNumber = 2 * yStepsNumber;
	int mainMaxNumber = 4 * (yStepsNumber * zStepsNumber + yStepsNumber * xStepsNumber);

	int main0 = 4 * (yStepsNumber * zStepsNumber) + 2 * yStepsNumber * xStepsNumber + 1;
	int side0 = mainMaxNumber + 2 * zStepsNumber * xStepsNumber - 2 * zStepsNumber + 2;
	for (int i = 0; i < xStepsNumber; i++) {
		internalGrid[main0].neighbours[2] = side0;
		internalGrid[side0].neighbours[2] = main0;
		main0 = main0 + doubledYStepsNumber;
		side0 = side0 - 2 * zStepsNumber;
	}

	int mainM = 4 * (yStepsNumber * zStepsNumber) + 2 * yStepsNumber * xStepsNumber + doubledYStepsNumber;
	int sideM = mainMaxNumber + 4 * zStepsNumber * xStepsNumber - 2 * zStepsNumber + 2;
	for (int i = 0; i < xStepsNumber; i++) {
		internalGrid[mainM].neighbours[1] = sideM;
		internalGrid[sideM].neighbours[0] = mainM;
		mainM = mainM + doubledYStepsNumber;
		sideM = sideM - 2 * zStepsNumber;
	}
}

void ParallelepipedGenerator::createExternalConnections0Y0() {
	int doubledYStepsNumber = 2 * yStepsNumber;
	int mainMaxNumber = 4 * (yStepsNumber * zStepsNumber + yStepsNumber * xStepsNumber);

	int start = 2;
	int end = mainMaxNumber - 2 * yStepsNumber + 1;
	for (int i = 0; i < yStepsNumber; i++) {
		internalGrid[start].neighbours[0] = end;
		internalGrid[end].neighbours[1] = start;
		start = start + 2;
		end = end + 2;
	}
}

void ParallelepipedGenerator::createExternalConnections() {
	createExternalConnections0YZ();
	createExternalConnectionsMXZ();
	createExternalConnectionsMYZ();
	createExternalConnections0XZ();
	createExternalConnections0Y0();
}

void ParallelepipedGenerator::createConnections() {
	createInternalConnections();
	createExternalConnections();
}

void ParallelepipedGenerator::createInternalGrid() {
	createTriangles();
	createConnections();
}

vector<Node> ParallelepipedGenerator::getNodes() {
	return nodes;
}

vector<Triangle> ParallelepipedGenerator::getGrid() {
	return internalGrid;
}