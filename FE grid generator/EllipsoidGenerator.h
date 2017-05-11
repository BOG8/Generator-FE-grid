#include "Node.h"
#include "Triangle.h"
#include "Tetrahedron.h"
#include <string>
#include <vector>
#include <list>

using namespace std;

const double PI = 3.1415926535897932384626433832795;

class EllipsoidGenerator {
	double a, b, c;
	int thetaLevel, phiLevel, internalLevel;
	int maxNumber;

	vector<double> thetaValues;
	vector<double> phiValues;
	Node *** arrayOfNodes;
	vector<Node> axisZNodes;
	vector<Triangle> externalGrid;
	list<Tetrahedron> tetrahedrons;
	vector<Node> internalNodes;
	vector<Node> nodes;

public:
	EllipsoidGenerator();
	~EllipsoidGenerator();

	void loadEllipsoidProperties(string fileName);
	void createAllNodes();
	void createExternalGrid();
	void createTriangulation();
	void writeNodesInFile(string fileName);
	void writeTetrahedronsInFile(string fileName);

private:
	void allocateMemory();
	void fillVectors();
	void createMainExternalNodes();
	void createMainInternalNodes();
	void reflectByAxisX();
	void reflectByAxisY();
	void reflectByAxisZ();
	void createAxisZNodes();
	void numberNodes();
	void printNodes();

	void createExternalGridTopConnections(int phiIndex);
	void createExternalGridTop();
	void createExternalGridMiddleConnections(int thetaIndex, int phiIndex);
	void createExternalGridMiddle();
	void createExternalGridBottomConnections(int thetaIndex, int phiIndex);
	void createExternalGridBottom();

	void createInitialTriangulation();
	void transferInternalNodes();
	void transferAllNodes();
	int getStatusOfAccessoryToTetrahedron(Node node, Tetrahedron tetrahedron);
	int getIncrementedStatus(int status);
	void fillNeighours(vector<Tetrahedron*> &neighbours, vector<int> &basisRibNodes,
					   list<Tetrahedron>::iterator currentTetrahedron, int status);
	void addNewTetrahedronsWithNodes(vector<Tetrahedron*> &neighbours, vector<int> &basisRibNodes, int i);
	void deleteGarbage();

public:
	void addInternalNodesToTriangulation();
};