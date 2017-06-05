#include "DeloneRebuilder.h"

DeloneRebuilder::DeloneRebuilder() {

}

DeloneRebuilder::~DeloneRebuilder() {

}

double DeloneRebuilder::calculateDistance(Node N1, Node N2) {
	return sqrt(pow(N2.x - N1.x, 2) + pow(N2.y - N1.y, 2) + pow(N2.z - N1.z, 2));
}

bool DeloneRebuilder::isDeloneCondition(Node N1, Node N2, Node N3, Node N4, Node P) {
	double Q = (pow(N1.x, 2) - pow(N2.x, 2) + pow(N1.y, 2) - pow(N2.y, 2) + pow(N1.z, 2) - pow(N2.z, 2)) / (2 * (N1.x - N2.x));
	double G3 = ((N1.x - N2.x) * (N1.y - N3.y) - (N1.x - N3.x) * (N1.y - N2.y)) / ((N1.x - N2.x) * (N1.x - N3.x));
	double G4 = ((N1.x - N2.x) * (N1.y - N4.y) - (N1.x - N4.x) * (N1.y - N2.y)) / ((N1.x - N2.x) * (N1.x - N4.x));
	double H3 = ((N1.x - N3.x) * (N1.z - N2.z) - (N1.x - N2.x) * (N1.z - N3.z)) / ((N1.x - N2.x) * (N1.x - N3.x));
	double H4 = ((N1.x - N4.x) * (N1.z - N2.z) - (N1.x - N2.x) * (N1.z - N4.z)) / ((N1.x - N2.x) * (N1.x - N4.x));
	double F3 = (pow(N1.x, 2) - pow(N3.x, 2) + pow(N1.y, 2) - pow(N3.y, 2) + pow(N1.z, 2) - pow(N3.z, 2)) / (2 * (N1.x - N3.x));
	double F4 = (pow(N1.x, 2) - pow(N4.x, 2) + pow(N1.y, 2) - pow(N4.y, 2) + pow(N1.z, 2) - pow(N4.z, 2)) / (2 * (N1.x - N4.x));
	double C = (((F4 - Q) / G4) - ((F3 - Q) / G3)) / ((H3 / G3) - (H4 / G4));
	double B = (F3 - Q + C * H3) / G3;
	double A = Q - ((B * (N1.y - N2.y)) / (N1.x - N2.x)) - ((C * (N1.z - N2.z)) / (N1.x - N2.x));
	double R = pow((N1.x - A), 2) + pow((N1.y - B), 2) + pow((N1.z - C), 2);

	Node center;
	center.x = A;
	center.y = B;
	center.z = C;
	double distance = calculateDistance(center, P);

	if (distance >= R) {
		return true;
	}

	return false;
}

void DeloneRebuilder::definePlaneCoefficients(Node one, Node two, Node three) {
	double indX2 = two.x - one.x;
	double indX3 = three.x - one.x;
	double indY2 = two.y - one.y;
	double indY3 = three.y - one.y;
	double indZ2 = two.z - one.z;
	double indZ3 = three.z - one.z;

	planeA = indY2 * indZ3 - indY3 * indZ2;
	planeB = -(indX2 * indZ3 - indX3 * indZ2);
	planeC = indX2 * indY3 - indX3 * indY2;
	planeD = -one.x * planeA - one.y * planeB - one.z * planeC;
}

Node DeloneRebuilder::definePerpendicularPlaneNode(Node node) {
	double tCoeff = pow(planeA, 2) + pow(planeB, 2) + pow(planeC, 2);
	double coeff = planeA * node.x + planeB * node.y + planeC * node.z + planeD;
	double t = -(coeff / tCoeff);

	Node planeNode;
	planeNode.x = planeA * t + node.x;
	planeNode.y = planeB * t + node.y;
	planeNode.z = planeC * t + node.z;

	return planeNode;
}

Node DeloneRebuilder::defineVector(Node one, Node two) {
	Node node;
	node.x = two.x - one.x;
	node.y = two.y - one.y;
	node.z = two.z - one.z;

	return node;
}

Node DeloneRebuilder::multiplyNodeVector(Node nodeVector, double multiplier) {
	nodeVector.x = nodeVector.x * multiplier;
	nodeVector.y = nodeVector.y * multiplier;
	nodeVector.z = nodeVector.z * multiplier;

	return nodeVector;
}

Node DeloneRebuilder::sumNodes(Node one, Node two) {
	Node node;
	node.x = one.x + two.x;
	node.y = one.y + two.y;
	node.z = one.z + two.z;

	return node;
}

double DeloneRebuilder::defineABSCrossProduct(Node one, Node two) {
	double x = one.y * two.z - one.z * two.y;
	double y = one.z * two.x - one.x * two.z;
	double z = one.x * two.y - one.y * two.x;

	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

bool DeloneRebuilder::isAccessory(Node planeNode, Node A, Node B, Node C) {
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

	if (alfa > 0 && alfa < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1 && zero < 0.000001) {
		return true;
	}

	return false;
}

bool DeloneRebuilder::isCanBeRebuild(Node V1, Node V2, Node M1, Node M2, Node M3) {
	definePlaneCoefficients(M1, M2, M3);
	Node V1PlaneNode = definePerpendicularPlaneNode(V1);
	Node V2PlaneNode = definePerpendicularPlaneNode(V2);

	double V1Distance = calculateDistance(V1, V1PlaneNode);
	double V2Distance = calculateDistance(V2, V2PlaneNode);
	double sum = V2Distance + V1Distance;
	double multiplier = V1Distance / sum;
	Node nodeVector = defineVector(V1, V2);
	Node multipliedNodeVector = multiplyNodeVector(nodeVector, multiplier);

	Node planeNode = sumNodes(V1, multipliedNodeVector);
	return isAccessory(planeNode, M1, M2, M3);
}

bool DeloneRebuilder::isNeedToRebuild(Node V1, Node V2, Node M1, Node M2, Node M3) {
	if (isCanBeRebuild(V1, V2, M1, M2, M3) && 
		(!isDeloneCondition(V1, M1, M2, M3, V2) || !isDeloneCondition(V2, M1, M2, M3, V1))) {
		return true;
	}

	return false;
}

void DeloneRebuilder::defineNodesNumbersAndIndexes(int &V1, int &V2, vector<int> &mVect, vector<int> &mTVect, vector<int> &mBVect, Tetrahedron* tetr1, Tetrahedron* tetr2) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (tetr1->nodesNumbers[i] == tetr2->nodesNumbers[j]) {
				mVect.push_back(tetr1->nodesNumbers[i]);
				mTVect.push_back(i);
				mBVect.push_back(j);
				break;
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if ((i != mTVect[0]) && (i != mTVect[1]) && (i != mTVect[2])) {
			V1 = tetr1->nodesNumbers[i];
		}
	}
	for (int i = 0; i < 4; i++) {
		if ((i != mBVect[0]) && (i != mBVect[1]) && (i != mBVect[2])) {
			V2 = tetr2->nodesNumbers[i];
		}
	}
}

void DeloneRebuilder::rebuild(list<Tetrahedron> *tetrahedrons) {
	list<Tetrahedron>::iterator currentTetrahedron = tetrahedrons->begin();
	list<Tetrahedron>::iterator end = tetrahedrons->end();
	vector<Tetrahedron*> tetrs;
	while (currentTetrahedron != end) {
		tetrs.push_back(&(*currentTetrahedron));
		currentTetrahedron++;
	}

	for (int i = 0; i < tetrs.size(); i++) {
		if (tetrs[i]->neighbours.size() > 0) {
			for (int j = 0; j < 4; j++) {
				if (tetrs[i]->neighbours[j] != 0) {
					int V1Numb = -1;
					int V2Numb = -1;
					vector<int> mVect;
					vector<int> mTVect;
					vector<int> mBVect;
					defineNodesNumbersAndIndexes(V1Numb, V2Numb, mVect, mTVect, mBVect, tetrs[i], tetrs[i]->neighbours[j]);
					Node V1 = nodes[V1Numb];
					Node V2 = nodes[V2Numb];
					Node M1 = nodes[mVect[0]];
					Node M2 = nodes[mVect[1]];
					Node M3 = nodes[mVect[2]];
					if (isNeedToRebuild(V1, V2, M1, M2, M3)) {

						for (int k = 0; k < 3; k++) {
							Tetrahedron tetrahedron;
							tetrahedrons->push_back(tetrahedron);
						}
						list<Tetrahedron>::iterator lastTetrs = tetrahedrons->end();
						vector<Tetrahedron*> newTetrs;
						for (int k = 0; k < 3; k++) {
							lastTetrs--;
							newTetrs.push_back(&(*lastTetrs));
						}

						for (int k = 0; k < newTetrs.size(); k++) {
							newTetrs[k]->nodesNumbers.push_back(V1Numb);
						}
						newTetrs[0]->nodesNumbers.push_back(mVect[0]);
						newTetrs[0]->nodesNumbers.push_back(mVect[1]);
						newTetrs[1]->nodesNumbers.push_back(mVect[1]);
						newTetrs[1]->nodesNumbers.push_back(mVect[2]);
						newTetrs[2]->nodesNumbers.push_back(mVect[2]);
						newTetrs[2]->nodesNumbers.push_back(mVect[0]);
						for (int k = 0; k < newTetrs.size(); k++) {
							newTetrs[k]->nodesNumbers.push_back(V2Numb);
						}

						newTetrs[0]->neighbours.push_back(tetrs[i]->neighbours[j]->neighbours[mBVect[2]]);
						newTetrs[0]->neighbours.push_back(0);
						newTetrs[0]->neighbours.push_back(0);
						newTetrs[0]->neighbours.push_back(tetrs[i]->neighbours[mTVect[2]]);

						newTetrs[1]->neighbours.push_back(tetrs[i]->neighbours[j]->neighbours[mBVect[0]]);
						newTetrs[1]->neighbours.push_back(0);
						newTetrs[1]->neighbours.push_back(0);
						newTetrs[1]->neighbours.push_back(tetrs[i]->neighbours[mTVect[0]]);

						newTetrs[2]->neighbours.push_back(tetrs[i]->neighbours[j]->neighbours[mBVect[1]]);
						newTetrs[2]->neighbours.push_back(0);
						newTetrs[2]->neighbours.push_back(0);
						newTetrs[2]->neighbours.push_back(tetrs[i]->neighbours[mTVect[1]]);

						//createInternalConnections;

						tetrs[i]->neighbours[j]->neighbours.clear();
						tetrs[i]->neighbours[j]->ribs.clear();
						tetrs[i]->neighbours[j]->nodesNumbers.clear();
						tetrs[i]->neighbours.clear();
						tetrs[i]->ribs.clear();
						tetrs[i]->nodesNumbers.clear();

						break;
					}
				}
			}
		}
	}
}

void DeloneRebuilder::setNodes(vector<Node> nodes) {
	this->nodes = nodes;
}