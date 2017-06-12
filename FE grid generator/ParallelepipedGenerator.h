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
	vector<Node> nodes;
	vector<Node> internalNodes;
	vector<Triangle> grid0YZ;
	vector<Triangle> gridMYZ;
	vector<Triangle> grid0XZ;
	vector<Triangle> gridMXZ;
	vector<Triangle> grid0XY;
	vector<Triangle> gridMXY;

public:
	ParallelepipedGenerator();
	~ParallelepipedGenerator();

	void setMinNumber(int number);
	void loadEllipsoidProperties(string fileName);
	void correctAllSteps();
	void createNodes(string fileName);
	void transferAllNodes();
	void transferInternalNodes();
	void createInternalGrid();
	void writeAneuFile(int defectGridSize);

	vector<Node> getNodes();
	vector<Node> getInternalNodes();
	vector<Triangle> getGrid();

private:
	void correctStep(double &x, double &xStep, int &xStepsNumber);
	bool isBelongsToEllipsoid(Node node);
	void printNodesNumbers();
	void createTriangles0ZY();
	void createTrianglesMXY();
	void createTrianglesMZY();
	void createTriangles0XY();
	void createTriangles0XZ();
	void createTrianglesMXZ();
	void createTriangles();
	
	void createInternalXZConnections(int index);
	void createInternalConnections();
	void createExternalConnections0YZ();
	void createExternalConnectionsMXZ();
	void createExternalConnectionsMYZ();
	void createExternalConnections0XZ();
	void createExternalConnections0Y0();
	void createExternalConnections();
	void createConnections();

	void createInternalNodes();
};