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
	vector<Node> getNodes();
	list<Tetrahedron>* getTetrahedrons();
	void writeAneuFile(int defectTetrsNumb);
	void writeAneuGridFile();
	int getInternalGridSize();

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
	void createTetrahedronsFromRibsToRibs(vector<Rib> &vectorOne, vector<Rib> &vectorTwo);
	void fillEmptyAreas();
	bool isVectorContainsNumber(vector<int> vector, int number);
	void fillVectorOfNodesNumbers(vector<int> &curNodesNumbers, Tetrahedron tetr, int index);
	void createConnections(list<Tetrahedron>::iterator currentTetrahedron, list<Tetrahedron>::iterator end);
	void createConnectionsForAllTetrahedrons();
	double defineDeterminant3spc(Node N1, Node N2, Node N3);
	double defineDeterminant4spc(Node N1, Node N2, Node N3, Node N4);
	int isNodeBelongsToTetrahedron(Node node, Tetrahedron tetr);
	int defineIndexFromDifferentNode(Tetrahedron* tetr, int N1, int N2, int N3, int N4);
	void addInternalNodesToTriangulation();
};