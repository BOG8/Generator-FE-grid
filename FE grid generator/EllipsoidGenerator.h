#include "Triangle.h"
#include <string>
#include <vector>

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

public:
	EllipsoidGenerator();
	~EllipsoidGenerator();

	void loadEllipsoidProperties(string fileName);
	void createAllNodes();
	void createExternalGrid();

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
};