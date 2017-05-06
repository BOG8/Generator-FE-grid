#include "Node.h"
#include <string>
#include <vector>

using namespace std;

const double PI = 3.1415926535897932384626433832795;

class EllipsoidGenerator {
	double a, b, c;
	double r;
	int thetaLevel, phiLevel, internalLevel;

	vector<double> thetaValues;
	vector<double> phiValues;
	Node *** arrayOfNodes;

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
	void printNodes();
};