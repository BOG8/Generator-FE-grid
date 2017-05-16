#include "Node.h"
#include "Triangle.h"
#include "EllipsoidGenerator.h"
#include <string>
#include <vector>

using namespace std;

class ParallelepipedGenerator {
	double x, y, z;
	double xStep, yStep, zStep;
	int xStepsNumber, yStepsNumber, zStepsNumber;
	int minNumber;

	EllipsoidGenerator ellipsoidGenerator;
	Node ***arrayOfNodes;
	vector<Triangle> internalGrid;

public:
	ParallelepipedGenerator();
	~ParallelepipedGenerator();

	void setMinNumber(int number);
	void loadEllipsoidProperties(string fileName);
	void correctAllSteps();
	void createNodes(string fileName);
	void createInternalGrid();

private:
	void correctStep(double &x, double &xStep, int &xStepsNumber);
	bool isBelongsToEllipsoid(Node node);
	void printNodesNumbers();
};