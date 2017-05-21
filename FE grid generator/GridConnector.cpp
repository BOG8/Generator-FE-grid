#include "GridConnector.h"
#include <math.h>

GridConnector::GridConnector() {

}

GridConnector::~GridConnector() {

}

void GridConnector::setData(vector<Node> nodes, vector<Node> intNd, vector<Triangle> extTr, vector<Triangle> intTr, Node node) {
	this->nodes = nodes;
	this->internalNodes = intNd;
	this->externalGrid = extTr;
	this->internalGrid = intTr;
	this->centerDefect = node;
}

void GridConnector::addEmptyTetrahedrons() {
	Tetrahedron tetrahedron;
	for (int i = 1; i < externalGrid.size(); i++) {
		tetrahedrons.push_front(tetrahedron);
	}
	for (int i = 1; i < internalGrid.size(); i++) {
		tetrahedrons.push_front(tetrahedron);
	}
}

void GridConnector::addBasisAndConnections() {
	list<Tetrahedron>::iterator currentTetrahedron = tetrahedrons.begin();
	for (int i = 1; i < externalGrid.size(); i++) {
		currentTetrahedron->nodesNumbers.push_back(-1);
		for (int j = 0; j < 3; j++) {
			currentTetrahedron->nodesNumbers.push_back(externalGrid[i].nodesNumbers[j]);
		}
		for (int j = 0; j < 4; j++) {
			currentTetrahedron->neighbours.push_back(0);
		}
		externalGrid[i].tetrahedron = &(*currentTetrahedron);

		currentTetrahedron++;
	}
	for (int i = 1; i < internalGrid.size(); i++) {
		currentTetrahedron->nodesNumbers.push_back(-1);
		for (int j = 0; j < 3; j++) {
			currentTetrahedron->nodesNumbers.push_back(internalGrid[i].nodesNumbers[j]);
		}
		for (int j = 0; j < 4; j++) {
			currentTetrahedron->neighbours.push_back(0);
		}
		internalGrid[i].tetrahedron = &(*currentTetrahedron);

		currentTetrahedron++;
	}
}

int GridConnector::getIncrementedIndex(int index) {
	index++;
	if (index == 3) {
		return 0;
	}

	return index;
}

bool GridConnector::isRibExist(Triangle triangle, vector<int> nodesNumbers) {
	for (int i = 0; i < externalRibs.size(); i++) {
		if (externalRibs[i].nodesNumbers[0] == nodesNumbers[1] && externalRibs[i].nodesNumbers[1] == nodesNumbers[0]) {
			externalRibs[i].neighbours.push_back(triangle.tetrahedron);
			triangle.tetrahedron->ribs.push_back(i);
			return true;
		}
	}

	return false;
}

void GridConnector::createExternalRibs(Triangle triangle) {
	for (int i = 0; i < 3; i++) {
		vector<int> nodesNumbers;
		nodesNumbers.push_back(triangle.nodesNumbers[i]);
		nodesNumbers.push_back(triangle.nodesNumbers[getIncrementedIndex(i)]);

		if (!isRibExist(triangle, nodesNumbers)) {
			Rib rib;
			rib.isEmptyArea = true;
			for (int j = 0; j < 2; j++) {
				rib.nodesNumbers.push_back(nodesNumbers[j]);
			}
			rib.neighbours.push_back(triangle.tetrahedron);
			externalRibs.push_back(rib);
			triangle.tetrahedron->ribs.push_back(externalRibs.size() - 1);
		}
	}
}

void GridConnector::definePlaneCoefficients(Node one, Node two, Node three) {
	double indX2 = two.x - one.x;
	double indX3 = three.x - one.x;
	double indY2 = two.y - one.y;
	double indY3 = three.y - one.y;
	double indZ2 = two.z - one.z;
	double indZ3 = three.z - one.z;

	double planeA = indY2 * indZ3 - indY3 * indZ2;
	double planeB = indX2 * indZ3 - indX3 * indZ2;
	double planeC = indX2 * indY3 - indX3 * indY2;
	double planeD = - one.x * planeA + one.y * planeB - one.z * planeC;
}

Node GridConnector::definePerpendicularPlaneNode(Node node) {
	double tCoeff = pow(planeA, 2) + pow(planeB, 2) + pow(planeC, 2);
	double coeff = planeA * node.x + planeB * node.y + planeC * node.y + planeD;
	double t = -(coeff / tCoeff);

	Node planeNode;
	planeNode.x = planeA * t + node.x;
	planeNode.y = planeB * t + node.y;
	planeNode.z = planeC * t + node.z;

	return planeNode;
}

double GridConnector::calculateDistance(Node one, Node two) {
	return sqrt(pow(two.x - one.x, 2) + pow(two.y - one.y, 2) + pow(two.z - one.z, 2));
}

Node GridConnector::defineNodeVector(Node one, Node two) {
	Node node;
	node.x = two.x - one.x;
	node.y = two.y - one.y;
	node.z = two.z - one.z;

	return node;
}

Node GridConnector::cutNodeVector(Node nodeVector, double multiplier) {
	nodeVector.x = nodeVector.x * multiplier;
	nodeVector.y = nodeVector.y * multiplier;
	nodeVector.z = nodeVector.z * multiplier;

	return nodeVector;
}

Node GridConnector::sumNodes(Node one, Node two) {
	Node node;
	node.x = one.x + two.x;
	node.y = one.y + two.y;
	node.z = one.z + two.z;
	
	return node;
}

void GridConnector::defineTop(Triangle triangle) {
	Node one = nodes[triangle.nodesNumbers[0]];
	Node two = nodes[triangle.nodesNumbers[1]];
	Node three = nodes[triangle.nodesNumbers[2]];

	Node triangleCenter;
	triangleCenter.x = (one.x + two.x + three.x) / 3;
	triangleCenter.y = (one.y + two.y + three.y) / 3;
	triangleCenter.z = (one.z + two.z + three.z) / 3;

	for (int i = 0; i < internalGrid.size; i++) {
		Node oneGrid = nodes[triangle.nodesNumbers[0]];
		Node twoGrid = nodes[triangle.nodesNumbers[1]];
		Node threeGrid = nodes[triangle.nodesNumbers[2]];
		definePlaneCoefficients(oneGrid, twoGrid, threeGrid);

		Node triangleCenterPlaneNode = definePerpendicularPlaneNode(triangleCenter);
		Node centerDefectPlaneNode = definePerpendicularPlaneNode(centerDefect);

		double triangleCenterDistance = calculateDistance(triangleCenter, triangleCenterPlaneNode);
		double centerDefectDistance = calculateDistance(centerDefect, centerDefectPlaneNode);
		double sum = centerDefectDistance + triangleCenterDistance;
		double multiplier = triangleCenterDistance / sum;
		Node nodeVector = defineNodeVector(triangleCenter, centerDefect);
		Node cuttedNodeVector = cutNodeVector(nodeVector, multiplier);
		Node planeNode = sumNodes(triangleCenter, cuttedNodeVector);

		double distance = calculateDistance(triangleCenter, centerDefect);
		double distancePartOne = calculateDistance(triangleCenter, planeNode);
		double distancePartTwo = calculateDistance(centerDefect, planeNode);
		if (distancePartOne < distance && distancePartTwo < distance && /*isBelongToPlane*/) {

		}
	}
}

void GridConnector::raiseTetrahedrons() {
	for (int i = 1; i < externalGrid.size(); i++) {
		Triangle triangle = externalGrid[i];
		createExternalRibs(triangle);
		defineTop(triangle);
	}
}

void GridConnector::createTetrahedrons() {
	addEmptyTetrahedrons();
	addBasisAndConnections();
	raiseTetrahedrons();
}

void GridConnector::writeGridInGidFile() {
	ofstream file("GridResult.txt");
	file << "mesh dimension = 3 elemtype triangle nnode = 3\ncoordinates\n";
	for (int i = 1; i < nodes.size(); i++) {
		Node node = nodes[i];
		file << node.number << " " << node.x << ' ' << node.y << ' ' << node.z << '\n';
	}
	file << "end coordinates\nelements\n";
	for (int i = 1; i < internalGrid.size(); i++) {
		Triangle triangle = internalGrid[i];
		file << i << ' ' << triangle.nodesNumbers[0] << ' ' << triangle.nodesNumbers[1] << ' ' << triangle.nodesNumbers[2] << " 1\n";
	}
	int currentNumber = internalGrid.size() - 1;
	for (int i = 1; i < externalGrid.size(); i++) {
		currentNumber++;
		Triangle triangle = externalGrid[i];
		file << currentNumber << ' ' << triangle.nodesNumbers[0] << ' ' << triangle.nodesNumbers[1] << ' ' << triangle.nodesNumbers[2] << " 2\n";
	}
	file << "end elements";

	file.close();
}