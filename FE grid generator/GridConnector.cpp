#include "GridConnector.h"

GridConnector::GridConnector() {

}

GridConnector::~GridConnector() {

}

void GridConnector::setData(vector<Node> nodes, vector<Node> intNd, vector<Triangle> extTr, vector<Triangle> intTr) {
	this->nodes = nodes;
	this->internalNodes = intNd;
	this->externalGrid = extTr;
	this->internalGrid = intTr;
}

void GridConnector::writeGridInGidFile() {
	ofstream file("GridResult.txt");
	file << "mesh dimension = 3 elemtype triangle nnode = 3\ncoordinates\n";
	for (int i = 1; i < nodes.size(); i++) {
		Node node = nodes[i];
		file << node.number << " " << node.x << ' ' << node.y << ' ' << node.z << '\n';
	}
	file << "end coordinates\nelements\n";
	for (int i = 1; i < internalGrid.size(); i++) {
		Triangle triangle = internalGrid[i];
		file << i << ' ' << triangle.nodesNumbers[0] << ' ' << triangle.nodesNumbers[1] << ' ' << triangle.nodesNumbers[2] << " 1\n";
	}
	int currentNumber = internalGrid.size() - 1;
	for (int i = 1; i < externalGrid.size(); i++) {
		currentNumber++;
		Triangle triangle = externalGrid[i];
		file << currentNumber << ' ' << triangle.nodesNumbers[0] << ' ' << triangle.nodesNumbers[1] << ' ' << triangle.nodesNumbers[2] << " 2\n";
	}
	file << "end elements";

	file.close();
}