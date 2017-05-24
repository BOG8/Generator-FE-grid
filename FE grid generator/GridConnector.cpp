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
	this->defectCenter = node;
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

	planeA = indY2 * indZ3 - indY3 * indZ2;
	planeB = -(indX2 * indZ3 - indX3 * indZ2);
	planeC = indX2 * indY3 - indX3 * indY2;
	planeD = - one.x * planeA - one.y * planeB - one.z * planeC;
}

Node GridConnector::definePerpendicularPlaneNode(Node node) {
	double tCoeff = pow(planeA, 2) + pow(planeB, 2) + pow(planeC, 2);
	double coeff = planeA * node.x + planeB * node.y + planeC * node.z + planeD;
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

Node GridConnector::defineVector(Node one, Node two) {
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

bool GridConnector::isBelongToPlane(Node node) {
	return abs(planeA * node.x + planeB * node.y + planeC * node.z + planeD) < 0.000001;
}

int GridConnector::definePlaneSide(Node node) {
	double result = planeA * node.x + planeB * node.y + planeC * node.z + planeD;
	if (result < -0.000001) {
		return -1;
	} else if (result > 0.000001) {
		return 1;
	} else {
		return 0;
	}
}

double GridConnector::defineABSCrossProduct(Node one, Node two) {
	double x = one.y * two.z - one.z * two.y;
	double y = one.z * two.x - one.x * two.z;
	double z = one.x * two.y - one.y * two.x;

	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

int GridConnector::defineIndexOfMaxNumber(double one, double two, double three) {
	if (one > two) {
		if (one > three) {
			return 0;
		}
	} else {
		if (two > three) {
			return 1;
		}
	}

	return 2;
}

Node GridConnector::defineTriangleCenterNode(Triangle triangle) {
	Node one = nodes[triangle.nodesNumbers[0]];
	Node two = nodes[triangle.nodesNumbers[1]];
	Node three = nodes[triangle.nodesNumbers[2]];

	Node triangleCenter;
	triangleCenter.x = (one.x + two.x + three.x) / 3;
	triangleCenter.y = (one.y + two.y + three.y) / 3;
	triangleCenter.z = (one.z + two.z + three.z) / 3;

	return triangleCenter;
}

Node GridConnector::definePlaneNode(Node triangleCenter) {
	Node triangleCenterPlaneNode = definePerpendicularPlaneNode(triangleCenter);
	Node centerDefectPlaneNode = definePerpendicularPlaneNode(defectCenter);

	double triangleCenterDistance = calculateDistance(triangleCenter, triangleCenterPlaneNode);
	double centerDefectDistance = calculateDistance(defectCenter, centerDefectPlaneNode);
	double sum = centerDefectDistance + triangleCenterDistance;
	double multiplier = triangleCenterDistance / sum;
	Node nodeVector = defineVector(triangleCenter, defectCenter);
	Node cuttedNodeVector = cutNodeVector(nodeVector, multiplier);

	return sumNodes(triangleCenter, cuttedNodeVector);
}

int GridConnector::defineAccessoryStatusOfNode(Node planeNode, Node A, Node B, Node C) {
	Node vectorAB = defineVector(A, B);
	Node vectorAC = defineVector(A, C);
	Node vectorPA = defineVector(planeNode, A);
	Node vectorPB = defineVector(planeNode, B);
	Node vectorPC = defineVector(planeNode, C);

	double areaABC = defineABSCrossProduct(vectorAC, vectorAB) / 2;
	double alfa = defineABSCrossProduct(vectorPB, vectorPC) / (2 * areaABC);
	double beta = defineABSCrossProduct(vectorPC, vectorPA) / (2 * areaABC);
	double gamma = defineABSCrossProduct(vectorPA, vectorPB) / (2 * areaABC);
	double zero = abs(alfa + beta + gamma - 1);

	if (alfa >= 0 && alfa <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1 && zero < 0.000001) {
		return defineIndexOfMaxNumber(alfa, beta, gamma);
	}

	return -1;
}

void GridConnector::defineTop(Triangle triangle) {
	Node triangleCenter = defineTriangleCenterNode(triangle);
	double distance = calculateDistance(triangleCenter, defectCenter);
	for (int i = 1; i < internalGrid.size(); i++) {
		Node oneGridNode = nodes[internalGrid[i].nodesNumbers[0]];
		Node twoGridNode = nodes[internalGrid[i].nodesNumbers[1]];
		Node threeGridNode = nodes[internalGrid[i].nodesNumbers[2]];
		definePlaneCoefficients(oneGridNode, twoGridNode, threeGridNode);

		int sideTriangleCenter = definePlaneSide(triangleCenter);
		int sideDefect = definePlaneSide(defectCenter);
		if (sideDefect != sideTriangleCenter && sideDefect != 0) {
			Node planeNode = definePlaneNode(triangleCenter);
			if (isBelongToPlane(planeNode)) {
				double distancePartOne = calculateDistance(triangleCenter, planeNode);
				double distancePartTwo = calculateDistance(defectCenter, planeNode);
				if (distancePartOne < distance && distancePartTwo < distance) {
					cout << ": " << i;
					int status = defineAccessoryStatusOfNode(planeNode, oneGridNode, twoGridNode, threeGridNode);
					if (status != -1) {
						cout << "!!!";
						triangle.tetrahedron->nodesNumbers[0] = internalGrid[i].nodesNumbers[status];
						break;
					}
				}
			}
		}
	}
}

void GridConnector::raiseTetrahedrons() {
	for (int i = 1; i < externalGrid.size(); i++) {
		Triangle triangle = externalGrid[i];
		createExternalRibs(triangle);
		cout << '\n' << i;
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

void GridConnector::writeGidFile() {
	ofstream file("GidEmptyZoneResult.txt");
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
		if (currentTetrahedron->nodesNumbers[0] != -1) {
			number++;
			file << number;
			for (int i = 0; i < 4; i++) {
				file << ' ' << currentTetrahedron->nodesNumbers[i];
			}

			file << " 1\n";
		}

		currentTetrahedron++;
	}
	file << "end elements";

	file.close();
}