#include "Node.h"
#include "Triangle.h"
#include "Tetrahedron.h"
#include <list>

class GridConnector {
	vector<Node> nodes;
	vector<Node> internalNodes;
	vector<Triangle> externalGrid;
	vector<Triangle> internalGrid;
	list<Tetrahedron> tetrahedrons;

public:
	GridConnector();
	~GridConnector();
};