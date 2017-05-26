#include "Node.h"
#include "Rib.h"
#include "Triangle.h"
#include "Tetrahedron.h"
#include <iostream>
#include <fstream>
#include <list>

class GridConnector {
	Node defectCenter;
	double planeA, planeB, planeC, planeD;
	vector<Node> nodes;
	vector<Rib> externalRibs;
	vector<Rib> internalRibs;
	vector<Node> internalNodes;
	vector<Triangle> externalGrid;
	vector<Triangle> internalGrid;
	list<Tetrahedron> tetrahedrons;

public:
	GridConnector();
	~GridConnector();

	void setData(vector<Node> nodes, vector<Node> intNd, vector<Triangle> extTr, vector<Triangle> intTr, Node node);
	void createTetrahedrons();
	void writeGridInGidFile();
	void writeGidFile();

private:
	void addEmptyTetrahedrons();
	void addBasisAndConnections();
	int getIncrementedIndex(int index);
	bool isRibExist(Triangle triangle, vector<int> nodesNumbers, vector<Rib> &ribs);
	void createRibs(Triangle triangle, vector<Rib> &ribs);
	void definePlaneCoefficients(Node one, Node two, Node three);
	Node definePerpendicularPlaneNode(Node triangleCenter);
	double calculateDistance(Node one, Node two);
	Node defineVector(Node one, Node two);
	Node multiplyNodeVector(Node nodeVector, double multiplier);
	Node sumNodes(Node one, Node two);
	bool isBelongToPlane(Node node);
	int definePlaneSide(Node node);
	double defineABSCrossProduct(Node one, Node two);
	int defineIndexOfMaxNumber(double one, double two, double three);
	Node defineTriangleCenterNode(Triangle triangle);
	Node defineTopPlaneNode(Node triangleCenter);
	int defineAccessoryStatusOfNode(Node planeNode, Node A, Node B, Node C);
	void defineTop(Triangle triangle);
	void raiseTetrahedrons();
	Node defineBotPlaneNode(Node triangleCenter, double distance);
	void defineBot(Triangle triangle);
	void omitTetrahedrons();
	void deleteRibsWithoutEmptyAreas(vector<Rib> &vector);
	void fillEmptyAreas();
};