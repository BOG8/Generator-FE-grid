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

bool GridConnector::isRibExist(Triangle triangle, vector<int> nodesNumbers, vector<Rib> &ribs) {
	for (int i = 0; i < ribs.size(); i++) {
		if (ribs[i].nodesNumbers[0] == nodesNumbers[1] && ribs[i].nodesNumbers[1] == nodesNumbers[0]) {
			ribs[i].neighbours.push_back(triangle.tetrahedron);
			triangle.tetrahedron->ribs.push_back(i);
			return true;
		}
	}

	return false;
}

void GridConnector::createRibs(Triangle triangle, vector<Rib> &ribs) {
	for (int i = 0; i < 3; i++) {
		vector<int> nodesNumbers;
		nodesNumbers.push_back(triangle.nodesNumbers[i]);
		nodesNumbers.push_back(triangle.nodesNumbers[getIncrementedIndex(i)]);

		if (!isRibExist(triangle, nodesNumbers, ribs)) {
			Rib rib;
			rib.isEmptyArea = true;
			for (int j = 0; j < 2; j++) {
				rib.nodesNumbers.push_back(nodesNumbers[j]);
			}
			rib.neighbours.push_back(triangle.tetrahedron);
			ribs.push_back(rib);
			triangle.tetrahedron->ribs.push_back(ribs.size() - 1);
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

Node GridConnector::multiplyNodeVector(Node nodeVector, double multiplier) {
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

Node GridConnector::defineTopPlaneNode(Node triangleCenter) {
	Node triangleCenterPlaneNode = definePerpendicularPlaneNode(triangleCenter);
	Node defectCenterPlaneNode = definePerpendicularPlaneNode(defectCenter);

	double triangleCenterDistance = calculateDistance(triangleCenter, triangleCenterPlaneNode);
	double defectCenterDistance = calculateDistance(defectCenter, defectCenterPlaneNode);
	double sum = defectCenterDistance + triangleCenterDistance;
	double multiplier = triangleCenterDistance / sum;
	Node nodeVector = defineVector(triangleCenter, defectCenter);
	Node multipliedNodeVector = multiplyNodeVector(nodeVector, multiplier);

	return sumNodes(triangleCenter, multipliedNodeVector);
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
			Node planeNode = defineTopPlaneNode(triangleCenter);
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
		createRibs(triangle, externalRibs);
		cout << '\n' << i;
		defineTop(triangle);
	}
}

Node GridConnector::defineBotPlaneNode(Node triangleCenter, double distance) {
	Node triangleCenterPlaneNode = definePerpendicularPlaneNode(triangleCenter);
	Node defectCenterPlaneNode = definePerpendicularPlaneNode(defectCenter);

	double triangleCenterDistance = calculateDistance(triangleCenter, triangleCenterPlaneNode);
	double defectCenterDistance = calculateDistance(defectCenter, defectCenterPlaneNode);
	double triangleCenterToPlaneNodeDistance = -distance * triangleCenterDistance / (triangleCenterDistance - defectCenterDistance);
	double sum = distance + triangleCenterToPlaneNodeDistance;
	double multiplier = sum / distance;
	Node nodeVector = defineVector(defectCenter, triangleCenter);
	Node multipliedNodeVector = multiplyNodeVector(nodeVector, multiplier);

	return sumNodes(defectCenter, multipliedNodeVector);
}

void GridConnector::defineBot(Triangle triangle) {
	Node triangleCenter = defineTriangleCenterNode(triangle);
	double distance = calculateDistance(triangleCenter, defectCenter);
	for (int i = 1; i < externalGrid.size(); i++) {
		Node oneGridNode = nodes[externalGrid[i].nodesNumbers[0]];
		Node twoGridNode = nodes[externalGrid[i].nodesNumbers[1]];
		Node threeGridNode = nodes[externalGrid[i].nodesNumbers[2]];
		definePlaneCoefficients(oneGridNode, twoGridNode, threeGridNode);

		Node planeNode = defineBotPlaneNode(triangleCenter, distance);
		if (isBelongToPlane(planeNode)) {
			double distancePartOne = calculateDistance(triangleCenter, planeNode);
			double distancePartTwo = calculateDistance(defectCenter, planeNode);
			if (distancePartOne < distancePartTwo && distance < distancePartTwo) {
				cout << ": " << i;
				int status = defineAccessoryStatusOfNode(planeNode, oneGridNode, twoGridNode, threeGridNode);
				if (status != -1) {
					cout << "!!!";
					triangle.tetrahedron->nodesNumbers[0] = externalGrid[i].nodesNumbers[status];
					break;
				}
			}
		}
	}
}

void GridConnector::omitTetrahedrons() {
	for (int i = 1; i < internalGrid.size(); i++) {
		Triangle triangle = internalGrid[i];
		createRibs(triangle, internalRibs);
		cout << "\n\n" << i;
		defineBot(triangle);
	}
}

void GridConnector::deleteRibsWithoutEmptyAreas(vector<Rib> &vector) {
	int iter = -1;
	for (int i = 0; i < vector.size(); i++) {
		int numberOne = vector[i].neighbours[0]->nodesNumbers[0];
		int numberTwo = vector[i].neighbours[1]->nodesNumbers[0];
		iter++;
		cout << i << ": " << iter;
		if (numberOne == numberTwo) {
			cout << "-----";
			vector.erase(vector.begin() + i);
			i--;
		}
		cout << '\n';
	}
}

void GridConnector::createTetrahedronsFromRibsToRibs(vector<Rib> &v1, vector<Rib> &v2) {
	for (int i = 0; i < v1.size(); i++) {
		if (v1[i].isEmptyArea == true) {
			int v1Vertex1 = v1[i].neighbours[0]->nodesNumbers[0];
			int v1Vertex2 = v1[i].neighbours[1]->nodesNumbers[0];
			for (int j = 0; j < v2.size(); j++) {
				if (v2[j].isEmptyArea == true) {
					int v2Number1 = v2[j].nodesNumbers[0];
					int v2Number2 = v2[j].nodesNumbers[1];
					if ((v1Vertex1 == v2Number1 && v1Vertex2 == v2Number2) || 
						(v1Vertex1 == v2Number2 && v1Vertex2 == v2Number1)) {
						Tetrahedron tetrahedron;
						tetrahedron.nodesNumbers.push_back(v1[i].nodesNumbers[0]);
						tetrahedron.nodesNumbers.push_back(v1[i].nodesNumbers[1]);
						tetrahedron.nodesNumbers.push_back(v2Number1);
						tetrahedron.nodesNumbers.push_back(v2Number2);
						for (int j = 0; j < 4; j++) {
							tetrahedron.neighbours.push_back(0);
						}
						tetrahedrons.push_back(tetrahedron);
						v1[i].isEmptyArea = false;
						int v1Number1 = v1[i].nodesNumbers[0];
						int v1Number2 = v1[i].nodesNumbers[1];
						int v2Vertex1 = v2[j].neighbours[0]->nodesNumbers[0];
						int v2Vertex2 = v2[j].neighbours[1]->nodesNumbers[0];
						if ((v1Number1 == v2Vertex1 && v1Number2 == v2Vertex2) || 
							(v1Number1 == v2Vertex2 && v1Number2 == v2Vertex1)) {
							v2[j].isEmptyArea = false;
						}
						break;
					}
				}
			}
		}
	}
}

void GridConnector::fillEmptyAreas() {
	deleteRibsWithoutEmptyAreas(externalRibs);
	deleteRibsWithoutEmptyAreas(internalRibs);
	createTetrahedronsFromRibsToRibs(externalRibs, internalRibs);
	createTetrahedronsFromRibsToRibs(internalRibs, externalRibs);
}

bool GridConnector::isVectorContainsNumber(vector<int> vector, int number) {
	for (int i = 0; i < vector.size(); i++) {
		if (vector[i] == number) {
			return true;
		}
	}

	return false;
}

void GridConnector::fillVectorOfNodesNumbers(vector<int> &curNodesNumbers, Tetrahedron tetr, int index) {
	for (int i = 0; i < 4; i++) {
		if (i != index) {
			curNodesNumbers.push_back(tetr.nodesNumbers[i]);
		}
	}
}

void GridConnector::createConnections(list<Tetrahedron>::iterator currentTetrahedron, list<Tetrahedron>::iterator end) {
	for (int i = 0; i < 4; i++) {
		if (currentTetrahedron->neighbours[i] == 0) {
			vector<int> curNodesNumbers;
			fillVectorOfNodesNumbers(curNodesNumbers, *currentTetrahedron, i);
			list<Tetrahedron>::iterator itTetrahedron = currentTetrahedron;
			itTetrahedron++;
			while (itTetrahedron != end) {
				vector<bool> containVector;
				int matchCount = 0;
				for (int j = 0; j < 4; j++) {
					containVector.push_back(isVectorContainsNumber(curNodesNumbers, itTetrahedron->nodesNumbers[j]));
					if (containVector[j]) {
						matchCount++;
					}
				}
				if (matchCount == 3) {
					bool exitKey = false;
					for (int j = 0; j < 4; j++) {
						if (!containVector[j]) {
							currentTetrahedron->neighbours[i] = &(*itTetrahedron);
							itTetrahedron->neighbours[j] = &(*currentTetrahedron);
							exitKey = true;
							break;
						}
					}
					if (exitKey) {
						break;
					}
				}

				itTetrahedron++;
			}
		}
	}
}

void GridConnector::createConnectionsForAllTetrahedrons() {
	list<Tetrahedron>::iterator currentTetrahedron = tetrahedrons.begin();
	list<Tetrahedron>::iterator end = tetrahedrons.end();
	int ww = 0;

	while (currentTetrahedron != end) {
		ww++;
		cout << "\n" << ww;
		createConnections(currentTetrahedron, end);
		currentTetrahedron++;
	}
}

double GridConnector::defineDeterminant3spc(Node N1, Node N2, Node N3) {
	double plus = (N1.x * N2.y * N3.z) + (N1.y * N2.z * N3.x) + (N1.z * N2.x * N3.y);
	double minus = -(N1.z * N2.y * N3.x) - (N1.y * N2.x * N3.z) - (N1.x * N2.z * N3.y);
	return plus * 1000 + minus * 1000;
	//return N1.y * N2.z + N1.z * N3.y + N2.y * N3.z - N2.z* N3.y - N1.y * N3.z - N1.z * N2.y;
}

double GridConnector::defineDeterminant4spc(Node N1, Node N2, Node N3, Node N4) {
	return N1.x * defineDeterminant3spc(N2, N3, N4) - N2.x * defineDeterminant3spc(N1, N3, N4) +
		N3.x * defineDeterminant3spc(N1, N2, N4) - N4.x * defineDeterminant3spc(N1, N2, N3);
}

int GridConnector::isNodeBelongsToTetrahedron(Node node, Tetrahedron tetr) {
	Node N1 = nodes[tetr.nodesNumbers[0]];
	Node N2 = nodes[tetr.nodesNumbers[1]];
	Node N3 = nodes[tetr.nodesNumbers[2]];
	Node N4 = nodes[tetr.nodesNumbers[3]];

	Node nN1 = defineVector(node, N1);
	Node nN2 = defineVector(node, N2);
	Node nN3 = defineVector(node, N3);
	Node nN4 = defineVector(node, N4);
	Node N1N2 = defineVector(N1, N2);
	Node N1N3 = defineVector(N1, N3);
	Node N1N4 = defineVector(N1, N4);

	double T = abs(defineDeterminant3spc(N1N2, N1N3, N1N4));
	double A = abs(defineDeterminant3spc(nN1, nN2, nN3)) / T;
	double B = abs(defineDeterminant3spc(nN1, nN2, nN4)) / T;
	double C = abs(defineDeterminant3spc(nN1, nN3, nN4)) / T;
	double D = abs(defineDeterminant3spc(nN2, nN3, nN4)) / T;
	double zero = abs(1 - A - B - C - D);

	if (A < 0.99999999 && B < 0.99999999 && C < 0.99999999 && D < 0.99999999 && zero < 0.00000001) {
		return 1;
	}
	if (A <= 1 && B <= 1 && C <= 1 && D <= 1 && zero < 0.00000001) {
		return 0;
	}
	//double D0 = defineDeterminant4spc(N1, N2, N3, N4);
	//double D1 = defineDeterminant4spc(node, N2, N3, N4);
	//double D2 = defineDeterminant4spc(N1, node, N3, N4);
	//double D3 = defineDeterminant4spc(N1, N2, node, N4);
	//double D4 = defineDeterminant4spc(N1, N2, N3, node);

	//if ((D0 > 0 && D1 > 0 && D2 > 0 && D3 > 0 && D4 > 0) || 
	//	(D0 < 0 && D1 < 0 && D2 < 0 && D3 < 0 && D4 < 0)) {
	//	return 1;
	//}
	//if ((D0 > 0 && D1 >= 0 && D2 >= 0 && D3 >= 0 && D4 >= 0) ||
	//	(D0 < 0 && D1 <= 0 && D2 <= 0 && D3 <= 0 && D4 <= 0)) {
	//	return 0;
	//}


	//definePlaneCoefficients(N1, N3, N2);
	//int one = definePlaneSide(node);
	//definePlaneCoefficients(N1, N2, N4);
	//int two = definePlaneSide(node);
	//definePlaneCoefficients(N1, N4, N3);	
	//int three = definePlaneSide(node);
	//definePlaneCoefficients(N2, N3, N4);	
	//int four = definePlaneSide(node);

	//if ((one > 0 && two > 0 && three > 0 && four > 0) ||
	//	(one < 0 && two < 0 && three < 0 && four < 0)) {
	//	return 1;
	//}

	return -1;
}

int GridConnector::defineIndexFromDifferentNode(Tetrahedron* tetr, int N1, int N2, int N3, int N4) {
	for (int i = 0; i < 4; i++) {
		int nodeNumb = tetr->nodesNumbers[i];
		if (nodeNumb != N1 && nodeNumb != N2 && nodeNumb != N3 && nodeNumb != N4) {
			return i;
		}
	}

	return -1;
}

void GridConnector::addInternalNodesToTriangulation() {
	list<Tetrahedron>::iterator end = tetrahedrons.end();

	for (int i = 0; i < internalNodes.size(); i++) {
		list<Tetrahedron>::iterator currentTetrahedron = tetrahedrons.begin();
		while (currentTetrahedron != end) {
			int status = isNodeBelongsToTetrahedron(internalNodes[i], *currentTetrahedron);
			if (status == 1) {
				cout << "\n" << i << ": XD";
				int V1Numb = currentTetrahedron->nodesNumbers[0];
				int V2Numb = currentTetrahedron->nodesNumbers[1];
				int V3Numb = currentTetrahedron->nodesNumbers[2];
				int V4Numb = currentTetrahedron->nodesNumbers[3];

				for (int j = 0; j < 4; j++) {
					Tetrahedron tetrahedron;
					tetrahedrons.push_back(tetrahedron);
				}
				list<Tetrahedron>::iterator lastTetrs = tetrahedrons.end();
				vector<Tetrahedron*> newTetrs;
				for (int j = 0; j < 4; j++) {
					lastTetrs--;
					newTetrs.push_back(&(*lastTetrs));
				}

				for (int j = 0; j < 4; j++) {
					newTetrs[j]->nodesNumbers.push_back(internalNodes[i].number);
				}
				newTetrs[0]->nodesNumbers.push_back(V1Numb);
				newTetrs[0]->nodesNumbers.push_back(V4Numb);
				newTetrs[0]->nodesNumbers.push_back(V2Numb);
				newTetrs[1]->nodesNumbers.push_back(V2Numb);
				newTetrs[1]->nodesNumbers.push_back(V4Numb);
				newTetrs[1]->nodesNumbers.push_back(V3Numb);
				newTetrs[2]->nodesNumbers.push_back(V3Numb);
				newTetrs[2]->nodesNumbers.push_back(V4Numb);
				newTetrs[2]->nodesNumbers.push_back(V1Numb);	
				newTetrs[3]->nodesNumbers.push_back(V1Numb);
				newTetrs[3]->nodesNumbers.push_back(V2Numb);
				newTetrs[3]->nodesNumbers.push_back(V3Numb);

				newTetrs[0]->neighbours.push_back(currentTetrahedron->neighbours[2]);
				if (currentTetrahedron->neighbours[2] != 0) {
					int newTetrt0Index = defineIndexFromDifferentNode(currentTetrahedron->neighbours[2], internalNodes[i].number, V1Numb, V4Numb, V2Numb);
					currentTetrahedron->neighbours[2]->neighbours[newTetrt0Index] = newTetrs[0];
				}
				newTetrs[0]->neighbours.push_back(newTetrs[1]);
				newTetrs[0]->neighbours.push_back(newTetrs[3]);
				newTetrs[0]->neighbours.push_back(newTetrs[2]);

				newTetrs[1]->neighbours.push_back(currentTetrahedron->neighbours[0]);
				if (currentTetrahedron->neighbours[0] != 0) {
					int newTetrt1Index = defineIndexFromDifferentNode(currentTetrahedron->neighbours[0], internalNodes[i].number, V2Numb, V4Numb, V3Numb);
					currentTetrahedron->neighbours[0]->neighbours[newTetrt1Index] = newTetrs[1];
				}
				newTetrs[1]->neighbours.push_back(newTetrs[2]);
				newTetrs[1]->neighbours.push_back(newTetrs[3]);
				newTetrs[1]->neighbours.push_back(newTetrs[0]);

				newTetrs[2]->neighbours.push_back(currentTetrahedron->neighbours[1]);
				if (currentTetrahedron->neighbours[1] != 0) {
					int newTetrt2Index = defineIndexFromDifferentNode(currentTetrahedron->neighbours[1], internalNodes[i].number, V3Numb, V4Numb, V1Numb);
					currentTetrahedron->neighbours[1]->neighbours[newTetrt2Index] = newTetrs[2];
				}
				newTetrs[2]->neighbours.push_back(newTetrs[0]);
				newTetrs[2]->neighbours.push_back(newTetrs[3]);
				newTetrs[2]->neighbours.push_back(newTetrs[1]);

				newTetrs[3]->neighbours.push_back(currentTetrahedron->neighbours[3]);
				if (currentTetrahedron->neighbours[3] != 0) {
					int newTetrt3Index = defineIndexFromDifferentNode(currentTetrahedron->neighbours[3], internalNodes[i].number, V1Numb, V2Numb, V3Numb);
					currentTetrahedron->neighbours[3]->neighbours[newTetrt3Index] = newTetrs[3];
				}
				newTetrs[3]->neighbours.push_back(newTetrs[1]);
				newTetrs[3]->neighbours.push_back(newTetrs[2]);
				newTetrs[3]->neighbours.push_back(newTetrs[3]);

				break;
			}
			else if (status == 0) {
				cout << "\n" << i << ": XD------------------";
			}

			currentTetrahedron++;
		}
	}
}

void GridConnector::createTetrahedrons() {
	addEmptyTetrahedrons();
	addBasisAndConnections();
	raiseTetrahedrons();
	omitTetrahedrons();
	fillEmptyAreas();
	createConnectionsForAllTetrahedrons();
	addInternalNodesToTriangulation();
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

void GridConnector::writeAneuFile(int defectTetrsNumb) {
	ofstream file("aneuFileResult.txt");

	int sizeN = nodes.size();
	file << sizeN - 1 << " 3\n";
	for (int i = 1; i < sizeN; i++) {
		Node node = nodes[i];
		file << node.x << ' ' << node.y << ' ' << node.z << '\n';
	}

	file << '\n';

	file << tetrahedrons.size() + defectTetrsNumb << " 4\n";
	list<Tetrahedron>::iterator currentTetrahedron = tetrahedrons.begin();
	list<Tetrahedron>::iterator end = tetrahedrons.end();
	while (currentTetrahedron != end) {
		file << '1';
		for (int i = 0; i < 4; i++) {
			file << ' ' << currentTetrahedron->nodesNumbers[i];
		}
		file << '\n';

		currentTetrahedron++;
	}

	file.close();
}

void GridConnector::writeAneuGridFile() {
	ofstream file("aneuFileResult.txt", ios_base::app);

	for (int i = 1; i < internalGrid.size(); i++) {
		file << '4';
		for (int j = 0; j < 3; j++) {
			file << ' ' << internalGrid[i].nodesNumbers[j];
		}
		file << '\n';
	}

	file.close();
}

vector<Node> GridConnector::getNodes() {
	return nodes;
}

list<Tetrahedron>* GridConnector::getTetrahedrons() {
	return &tetrahedrons;
}

int GridConnector::getInternalGridSize() {
	return internalGrid.size() - 1;
}