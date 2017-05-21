#include "ProcessManager.h"

ProcessManager::ProcessManager() {

}

ProcessManager::~ProcessManager() {

}

void ProcessManager::runEllipsoidGenerator() {
	ellipsoidGenerator.loadEllipsoidProperties(ELLIPSOID_PROPERTIES);
	ellipsoidGenerator.createAllNodes();
	ellipsoidGenerator.createExternalGrid();
	ellipsoidGenerator.createTriangulation();
	ellipsoidGenerator.writeNodesInFile(RESULT);
	ellipsoidGenerator.writeTetrahedronsInFile(RESULT);
	ellipsoidGenerator.writeGidFile();
}

void ProcessManager::runParallelepipedGenerator() {
	parallelepipedGenerator.setMinNumber(ellipsoidGenerator.getMaxNumber());
	parallelepipedGenerator.loadEllipsoidProperties(PARALLELEPIPED_PROPERTIES);
	parallelepipedGenerator.correctAllSteps();
	parallelepipedGenerator.createNodes(ELLIPSOID_PROPERTIES);
	parallelepipedGenerator.transferAllNodes();
	parallelepipedGenerator.createInternalGrid();
}

void ProcessManager::runGridConnector() {
	vector<Node> nodes = ellipsoidGenerator.getNodes();
	vector<Node> otherNodes = parallelepipedGenerator.getNodes();
	for (int i = 1; i < otherNodes.size(); i++) {
		nodes.push_back(otherNodes[i]);
	}
	otherNodes.clear();
	vector<Node> emptyVector;
	gridConnector.setData(nodes, emptyVector, parallelepipedGenerator.getGrid(), ellipsoidGenerator.getGrid(), ellipsoidGenerator.getDefect());
	gridConnector.createTetrahedrons();
	gridConnector.writeGridInGidFile();
}

void ProcessManager::run() {
	runEllipsoidGenerator();
	runParallelepipedGenerator();
	runGridConnector();
}