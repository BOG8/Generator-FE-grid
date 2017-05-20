#include "Node.h"
#include "Rib.h"
#include "Triangle.h"
#include "Tetrahedron.h"
#include <iostream>
#include <fstream>
#include <list>

class GridConnector {
	vector<Node> nodes;
	vector<Rib> externalRibs;
	vector<Node> internalNodes;
	vector<Triangle> externalGrid;
	vector<Triangle> internalGrid;
	list<Tetrahedron> tetrahedrons;

public:
	GridConnector();
	~GridConnector();

	void setData(vector<Node> nodes, vector<Node> intNd, vector<Triangle> extTr, vector<Triangle> intTr);
	void createTetrahedrons();
	void writeGridInGidFile();

private:
	void addEmptyTetrahedrons();
	void addBasisAndConnections();
	int getIncrementedIndex(int index);
	bool isRibExist(Triangle triangle, vector<int> nodesNumbers);
	void raiseTetrahedrons();
};