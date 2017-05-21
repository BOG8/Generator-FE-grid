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

void GridConnector::addEmptyTetrahedrons() {
	Tetrahedron tetrahedron;
	for (int i = 1; i < externalGrid.size(); i++) {
		tetrahedrons.push_front(tetrahedron);
	}
	for (int i = 1; i < internalGrid.size(); i++) {
		tetrahedrons.push_front(tetrahedron);
	}
}

void GridConnector::addBasisAndConnections() {
	list<Tetrahedron>::iterator currentTetrahedron = tetrahedrons.begin();
	for (int i = 1; i < externalGrid.size(); i++) {
		currentTetrahedron->nodesNumbers.push_back(-1);
		for (int j = 0; j < 3; j++) {
			currentTetrahedron->nodesNumbers.push_back(externalGrid[i].nodesNumbers[j]);
		}
		for (int j = 0; j < 4; j++) {
			currentTetrahedron->neighbours.push_back(0);
		}
		externalGrid[i].tetrahedron = &(*currentTetrahedron);

		currentTetrahedron++;
	}
	for (int i = 1; i < internalGrid.size(); i++) {
		currentTetrahedron->nodesNumbers.push_back(-1);
		for (int j = 0; j < 3; j++) {
			currentTetrahedron->nodesNumbers.push_back(internalGrid[i].nodesNumbers[j]);
		}
		for (int j = 0; j < 4; j++) {
			currentTetrahedron->neighbours.push_back(0);
		}
		internalGrid[i].tetrahedron = &(*currentTetrahedron);

		currentTetrahedron++;
	}
}

int GridConnector::getIncrementedIndex(int index) {
	index++;
	if (index == 3) {
		return 0;
	}

	return index;
}

bool GridConnector::isRibExist(Triangle triangle, vector<int> nodesNumbers) {
	for (int i = 0; i < externalRibs.size(); i++) {
		if (externalRibs[i].nodesNumbers[0] == nodesNumbers[1] && externalRibs[i].nodesNumbers[1] == nodesNumbers[0]) {
			externalRibs[i].neighbours.push_back(triangle.tetrahedron);
			triangle.tetrahedron->ribs.push_back(i);
			return true;
		}
	}

	return false;
}

void GridConnector::raiseTetrahedrons() {
	for (int i = 1; i < externalGrid.size(); i++) {
		Triangle triangle = externalGrid[i];
		for (int j = 0; j < 3; j++) {
			vector<int> nodesNumbers;
			nodesNumbers.push_back(triangle.nodesNumbers[j]);
			nodesNumbers.push_back(triangle.nodesNumbers[getIncrementedIndex(j)]);

			if (!isRibExist(triangle, nodesNumbers)) {
				Rib rib;
				rib.isEmptyArea = true;
				for (int j = 0; j < 2; j++) {
					rib.nodesNumbers.push_back(nodesNumbers[j]);
				}
				rib.neighbours.push_back(triangle.tetrahedron);
				externalRibs.push_back(rib);
				triangle.tetrahedron->ribs.push_back(externalRibs.size() - 1);
			}
		}
	}
}

void GridConnector::createTetrahedrons() {
	addEmptyTetrahedrons();
	addBasisAndConnections();
	raiseTetrahedrons();
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