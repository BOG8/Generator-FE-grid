#include "Node.h"
#include <string>
#include <vector>

using namespace std;

const double PI = 3.1415926535897932384626433832795;

class EllipsoidGenerator {
	double a, b, c;
	int thetaLevel, phiLevel, internalLevel;

	vector<double> thetaValues;
	vector<double> phiValues;
	Node *** arrayOfNodes;
	vector<Node> axisZNodes;

public:
	EllipsoidGenerator();
	~EllipsoidGenerator();

	void loadEllipsoidProperties(string fileName);
	void createAllNodes();

private:
	void allocateMemory();
	void fillVectors();
	void createMainExternalNodes();
	void createMainInternalNodes();
	void reflectByAxisX();
	void reflectByAxisY();
	void reflectByAxisZ();
	void createAxisZNodes();
	void printNodes();
};