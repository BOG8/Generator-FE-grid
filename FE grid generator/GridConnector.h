#include "Node.h"
#include "Rib.h"
#include "Triangle.h"
#include "Tetrahedron.h"
#include <iostream>
#include <fstream>
#include <list>

class GridConnector {
	Node centerDefect;
	double planeA, planeB, planeC, planeD;
	vector<Node> nodes;
	vector<Rib> externalRibs;
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

private:
	void addEmptyTetrahedrons();
	void addBasisAndConnections();
	int getIncrementedIndex(int index);
	bool isRibExist(Triangle triangle, vector<int> nodesNumbers);
	void createExternalRibs(Triangle triangle);
	void definePlaneCoefficients(Node one, Node two, Node three);
	Node definePerpendicularPlaneNode(Node triangleCenter);
	double calculateDistance(Node one, Node two);
	Node defineNodeVector(Node one, Node two);
	Node cutNodeVector(Node nodeVector, double multiplier);
	Node sumNodes(Node one, Node two);
	bool isBelongToPlane(Node node);
	int definePlaneSide(Node node);
	void defineTop(Triangle triangle);
	void raiseTetrahedrons();
};