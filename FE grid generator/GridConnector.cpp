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