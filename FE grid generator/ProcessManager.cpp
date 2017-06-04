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
	parallelepipedGenerator.transferInternalNodes();
	parallelepipedGenerator.createInternalGrid();
}

void ProcessManager::runGridConnector() {
	vector<Node> nodes = ellipsoidGenerator.getNodes();
	vector<Node> otherNodes = parallelepipedGenerator.getNodes();
	for (int i = 1; i < otherNodes.size(); i++) {
		nodes.push_back(otherNodes[i]);
	}
	otherNodes.clear();
	vector<Node> internalNodes = parallelepipedGenerator.getInternalNodes();
	gridConnector.setData(nodes, internalNodes, parallelepipedGenerator.getGrid(), ellipsoidGenerator.getGrid(), ellipsoidGenerator.getDefect());
	gridConnector.createTetrahedrons();
	gridConnector.writeGridInGidFile();
	gridConnector.writeGidFile();
}

void ProcessManager::run() {
	runEllipsoidGenerator();
	runParallelepipedGenerator();
	runGridConnector();
}